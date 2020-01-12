FROM balenalib/aarch64-ubuntu-python:latest-bionic

RUN [ "cross-build-start" ]

# Prepare and install ros2 eloquent
RUN install_packages curl gnupg2 lsb-release
RUN curl -s https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc | apt-key add -
RUN sh -c 'echo "deb [arch=amd64,arm64] http://packages.ros.org/ros2/ubuntu `lsb_release -cs` main" > /etc/apt/sources.list.d/ros2-latest.list'
RUN install_packages ros-eloquent-ros-base

# Prepare and install wiringpi
# Install add-apt-repository
RUN install_packages software-properties-common
RUN add-apt-repository ppa:ubuntu-pi-flavour-makers/ppa
RUN install_packages pigpio

RUN [ "cross-build-end" ]

