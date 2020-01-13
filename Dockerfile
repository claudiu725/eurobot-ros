FROM balenalib/aarch64-ubuntu:bionic

RUN [ "cross-build-start" ]

# Prepare and install ros melodic
RUN install_packages curl gnupg2 lsb-release
RUN curl -sSL 'http://keyserver.ubuntu.com/pks/lookup?op=get&search=0xC1CF6E31E6BADE8868B172B4F42ED6FBAB17C654' | sudo apt-key add -
RUN sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
RUN install_packages ros-melodic-ros-base
RUN install_packages ros-melodic-navigation

# Prepare and install wiringpi
# Install add-apt-repository
RUN install_packages software-properties-common
RUN add-apt-repository ppa:ubuntu-pi-flavour-makers/ppa
RUN install_packages pigpio

RUN install_packages build-essential
COPY . /app
RUN . /opt/ros/melodic/setup.sh; /app/script/clean && /app/script/build

RUN [ "cross-build-end" ]

