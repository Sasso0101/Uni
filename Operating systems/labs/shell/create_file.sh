#!/bin/bash
create_file() {
	mkdir $1
	cd $1
	touch ${2}.txt
}

create_file $1 $2
