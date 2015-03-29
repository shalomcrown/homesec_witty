#!/bin/bash
#################################################
#
# Run project for debugging
# Good for Ubuntu at least.
#
#################################################

[[ -n docroot/resources ]] && ln -s /usr/share/Wt/resources docroot/resources

src/homesec_witty --docroot docroot --http-address 0.0.0.0 --http-port 8080
