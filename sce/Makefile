sep = /
config_dir	= ..$(sep)config$(sep)
include $(config_dir)sfDefine.mk

all:
	cd $(platform);  $(save_exe_type);  $(MAKE) $@

libs:
	cd $(platform);  $(save_exe_type);  $(MAKE) libscen.a
	
parComp:
	cd $(platform);  $(save_exe_type);  $(MAKE) $@

remlink:
	cd $(platform);  $(save_exe_type);  $(MAKE) $@

clean:
	cd $(platform); $(MAKE) $@

depend:
	cd $(platform);  $(save_exe_type);  $(MAKE) $@
