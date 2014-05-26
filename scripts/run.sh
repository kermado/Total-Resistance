if [ "${PWD##*/}" == "build" ]; then
	cd ..
	./build/bin/Game
else
	echo "Sorry, this script must be run from the 'build' directory."
	echo "You are trying to run this script from the ${PWD##*/} directory"
	exit 1
fi
