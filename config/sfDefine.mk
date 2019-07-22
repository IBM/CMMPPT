# Definitions used to save the type of the last compilation
# when the compilation was started in a non-platform-specific directory.

exe_type_file := last_exe_type
save_exe_type := echo $(exe_type) > $(exe_type_file)
