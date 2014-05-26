if [ "${PWD##*/}" == "build" ]; then
    if [ "$(ls -A ./)" ]; then
        rm -r ./*
    fi
    exit 0
else
    echo "Sorry, this script must be run from the 'build' directory."
    exit 1
fi
