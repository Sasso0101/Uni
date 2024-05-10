-- Library declarations
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
-- A generic Master SPI interface
entity spi_master is
  generic
  (
    -- Number of bits to transfer
    NUM  : natural   := 8;
    CPOL : std_logic := '1';
    CPHA : std_logic := '1'
  );
  port
  (
    -- System clock
    clk : in std_logic;
    -- System reset (active low)
    res : in std_logic;
    -- Command: load the data to be sent
    load_data : in std_logic;
    -- Data: the data to be sent
    data : in std_logic_vector(NUM - 1 downto 0);
    -- Command: start data transmission
    start : in std_logic;
    -- The data currently in the shift register
    -- (useful for debug)
    shift_data : out std_logic_vector(NUM - 1 downto 0);
    -- SPI interface signals (Master)
    MOSI : out std_logic;
    MISO : in std_logic;
    SCK  : out std_logic;
    SS   : out std_logic
  );
end entity spi_master;