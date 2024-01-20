# Build guide
# 1. Enable Vivado utility:
#   . /opt/Vivado/Vivado/2023.2/settings64.sh
# 2. Copy files to server:
#   scp * user@host:~/calculator
# 3. Generate bitstream (run command on server):
#   vivado -mode batch -nolog -nojournal -source build.tcl
# 4. Download bitstream:
#   scp user@host:~/calculator/output/calculator.bit calculator.bit
# 5. Flash on device:
#   openFPGALoader -b basys3 -f calculator.bit

add_files ./
read_xdc "Basys3_Master.xdc"

synth_design -top calculator -part xc7a35tcpg236-1

opt_design
place_design
route_design

write_bitstream -force calculator.bit