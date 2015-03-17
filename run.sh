#!/bin/bash
#################################################
#
# Run project for debugging
# Good for Ubuntu at least.
#
#################################################

[[ -n resources ]] && ln -s /usr/share/Wt/resources .

src/homesec_witty --docroot . --http-address 0.0.0.0 --http-port 8080
