all: 
	cd game && $(MAKE)
	cd client && $(MAKE)
	cd server && $(MAKE)

clean : 
	cd game && $(MAKE) clean
	cd client && $(MAKE) clean
	cd server && $(MAKE) clean