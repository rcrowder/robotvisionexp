DIRS	= adse_imx21 ads_imx21 altep2c35 as3dev bf537 csb226 csb250 \
	csb272 csb281 csb335 csb337 csb350 csb360 csb431 csb437tl csb472 \
	csb535fs csb536fs csb625 csb637 csb650 csb655 csb726 csb737 \
	evalsh2 fads860 m68en302 mpc852t template v4 walnut

top:
	@echo "Usage: make {clobber | rebuild}"

clean:
	for dir in $(DIRS) ; \
	do \
		cd $$dir; make clean; cd ..; \
	done

clobber:
	for dir in $(DIRS) ; \
	do \
		cd $$dir; make clobber; rm -f _vimrc; cd ..; \
	done

rebuild:
	for dir in $(DIRS) ; \
	do \
		cd $$dir; make rebuild; cd ..; \
	done

vimrc:
	d2u ../umon_main/host/src/scripts/_vimrc
	for dir in $(DIRS) ; \
	do \
		cp ../umon_main/host/src/scripts/_vimrc $$dir; \
	done
	
