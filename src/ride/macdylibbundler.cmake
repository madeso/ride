message(STATUS "Cleaning up Ride.app")

# use this to move local dependencies to the app https://github.com/auriamg/macdylibbundler
# here is a graphical way to view the dependencies https://github.com/kwin/macdependency
# The command we want to run is dylibbundler -od -b -x ./Ride.app/Contents/MacOS/ride -d ./Ride.app/Contents/libs/
execute_process(COMMAND "")
