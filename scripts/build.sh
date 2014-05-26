if [ "${PWD##*/}" == "build" ]; then
	echo "== Generating Makefiles =="
	cmake -DCMAKE_BUILD_TYPE=Debug ..
	echo "\n== Building Makefiles =="
	make
else
	echo "Sorry, this script must be run from the 'build' directory."
	echo "You are trying to run this script from the ${PWD##*/} directory"
	exit 1
fi
