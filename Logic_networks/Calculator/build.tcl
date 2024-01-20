set outputDir ./output         
file mkdir $outputDir
#
# STEP#1: setup design sources and constraints
#
add_files calculator.vhd accumulator.vhd debouncer.vhd seven_segment_driver.vhd alu.vhd
read_xdc "Basys3_Master.xdc"

#
# STEP#2: run synthesis, report utilization and timing estimates, write checkpoint design
#
synth_design -top calculator -part xc7a35tcpg236-1

opt_design
place_design
# write_checkpoint -force $outputDir/post_place
route_design
# write_checkpoint -force $outputDir/post_route

write_bitstream -force $outputDir/calculator.bit