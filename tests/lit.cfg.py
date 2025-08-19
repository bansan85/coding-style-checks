import lit.formats
import os

config.name = "MyCheck"
config.test_format = lit.formats.ShTest(True)
config.suffixes = ['.cpp']
config.test_source_root = os.path.dirname(__file__)
config.test_exec_root = config.test_source_root

# Répertoire contenant ton plugin compilé (.so)
plugin_path = os.path.join(config.test_source_root, "..", "build", "libCodingStyleCheck.so")

# Variable substituable dans les tests : %clang_tidy_plugin
config.substitutions.append(("%clang_tidy_plugin", plugin_path))

