if [ "${PWD##*/}" == "build" ]; then
	cmake .. -Dtests=ON
	echo "\n== Building Makefiles =="
	make
	echo "\n== Running Unit Tests =="
	cd ..
	./build/bin/EngineTests
else
	echo "Sorry, this script must be run from the 'build' directory."
	echo "You are trying to run this script from the ${PWD##*/} directory"
	exit 1
fi
