# -*- mode: ruby -*-
# vi: set ft=ruby :

# Vagrant box for building Ubuntu PPA packages.
VAGRANTFILE_API_VERSION = "2"

Vagrant.configure(VAGRANTFILE_API_VERSION) do |config|
  config.vm.box = "puppetlabs/ubuntu-14.04-64-nocm"
  config.ssh.forward_agent = true
  config.vm.provision "shell",
    inline: "apt-get update -y && apt-get upgrade -y && apt-get install -y build-essential devscripts dput debhelper"
end
