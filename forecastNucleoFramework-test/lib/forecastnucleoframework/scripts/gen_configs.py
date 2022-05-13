from sys import stderr
import re
import shutil
from pathlib import Path
import configparser


Import("env")

# defining important paths
template_dir_path = Path('../config_templates')
config_dir_path = Path('../include/forecast/config')
platforms_ini_path = Path('./platforms.ini')
default_config_path = Path('./defaults/workbench.ini')

# checking for templates
if not template_dir_path.is_dir():
    print("The templates directory does not exists!", file=stderr)
    Exit(1)

# checking for the default configs
if not default_config_path.is_file():
    print("Default configs not provided!", file=stderr)
    Exit(2)

# creating the config directory
if config_dir_path.is_dir():
    shutil.rmtree(config_dir_path)

# Looking for the platformio.ini file
pio_init = configparser.ConfigParser()
pio_init_path = env["PROJECT_DIR"] / Path("platformio.ini")
pio_init.read(pio_init_path)

pioenv = "env:" + env["PIOENV"]

# checking for the platform to compile against
if not pio_init.has_option(pioenv, "forecast_platform"):
    print("Please provvide a forecast_platform option to your environment")
    Exit(5)

forecast_platform = pio_init.get(pioenv, "forecast_platform").strip()

platforms_ini = configparser.ConfigParser()
platforms_ini.read(platforms_ini_path)

platform_section = 'platform:' + forecast_platform

if not platforms_ini.has_section('platform:' + forecast_platform):
    print("forecast platform", forecast_platform, "not found!")
    Exit(6)

# if the platform does not have a default go into error
if not platforms_ini.has_option(platform_section, 'default_config'):
    print('the platform is not provviding a default config')
    Exit(7)

default_config_path = Path(platforms_ini.get(platform_section, 'default_config'))

if not default_config_path.is_file():
    print("cant find the default config file: ", default_config_path)
    Exit(8)

# Opening the default config file
config_def = configparser.ConfigParser()
config_def.read(str(default_config_path))

# check if the suggested board is the same of the actual board
if platforms_ini.has_option(platform_section, 'board'):
    board = platforms_ini.get(platform_section, 'board')
    if env['BOARD'] != board:
        print('WARNING: the suggested board for the platform ', 
              forecast_platform, 'is', board, "the compilation board is", 
              env['BOARD'], "!")


src_filter = ['+<*>', '-<.git/>', '-<.svn/>', '-<example/>',
                ' -<examples/>', '-<test/>', '-<tests/>']

if platforms_ini.has_option(platform_section, 'exclude_files'):
    # excluding paths from dir
    exclude_inc = platforms_ini.get(platform_section, 'exclude_files');

    for inc in exclude_inc.split(','):
        src_filter.append('-<' + inc.strip() + '>')

env.Replace(SRC_FILTER=src_filter)
print(env['SRC_FILTER'])

# Looking for the value "forecast_cofig" in the current pioenv
forecast_config = env["PROJECT_DIR"] / Path("forecast_config.ini")
if pio_init.has_option(pioenv, "forecast_config"):
    if Path(pio_init.get(pioenv, "forecast_config")).is_absolute():
        forecast_config = Path(pio_init.get(pioenv, "forecast_config"))
    else:
        forecast_config = \
            env["PROJECT_DIR"] / Path(pio_init.get(pioenv, "forecast_config"))

if not forecast_config.is_file():
    print("The file \"" +  str(forecast_config) +
          "\" does not exists! The default values will be used for"
          " the configurations")

# Opening the config user as configparser object
config_usr = configparser.ConfigParser()
config_usr.read(forecast_config)

# creating the config directory
config_dir_path.mkdir()

template_regex = re.compile("\$(\w+)\[(\w+)\]\$")

# foreach template file 
for path in template_dir_path.iterdir():
    if path.is_file():
        with path.open(mode='r') as template:
            content = template.read()
        
        for match in re.finditer(template_regex, content):
            section = ('forecast:' + match.group(1)).strip()
            option = match.group(2).strip()

            # Looking for the value in the config user
            if config_usr.has_option(section, option):
                value = config_usr.get(section, option)
            # Looking for the value in the config default
            elif config_def.has_option(section, option):
                value = config_def.get(section, option)
            # Error if it is not found
            else:
                print("The value:\"", option, "\" of the section \"", section,
                      "\" required by the template: \"", str(path), "\" ",
                      "is not been declared", file=stderr, sep="")
                Exit(4)
        
            # replacing the value
            content = content.replace(match.group(0), value)

        # creating the template
        with (config_dir_path / path.name).open('w') as config:
            config.write(content)
