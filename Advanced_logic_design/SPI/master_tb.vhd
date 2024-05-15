library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity spi_master_tb is
end entity;

architecture rtl of spi_master_tb is
  component spi_master is
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
      reset : in std_logic;
      -- Command: load the data to be sent
      load_data : in std_logic;
      -- Data: the data to be sent
      data : in std_logic_vector(7 downto 0);
      -- Data currently in shift register
      shift_data : out std_logic_vector( 7 downto 0 );
      -- Command: start data transmission
      start : in std_logic;
      -- SPI interface signals (Master)
      MOSI : out std_logic;
      MISO : in std_logic;
      SCK  : out std_logic;
      SS   : out std_logic
    );
  end component;

  signal clk : std_logic := '0';
  signal reset : std_logic := '0';
  signal load_data : std_logic := '0';
  signal data : std_logic_vector( 7 downto 0 ) := (others => '0');
  signal shift_data : std_logic_vector( 7 downto 0 ) := (others => '0');
  signal start : std_logic := '0';
  signal MOSI : std_logic := '0';
  signal MISO : std_logic := '0';
  signal SCK : std_logic := '0';
  signal SS : std_logic := '0';
begin
    spi_master_0: spi_master
    port map (
        clk => clk,
        reset => reset,
        load_data => load_data,
        data => data,
        shift_data => shift_data,
        start => start,
        MOSI => MOSI,
        MISO => MISO,
        SCK => SCK,
        SS => SS
    );

    clk <= not(clk) after 100 us;
    process begin
        reset <= '1';
        wait for 10 us;
        reset <= '0';
        data <= "01001010";
        load_data <= '1';
        wait for 10 us;
        load_data <= '0';
        start <= '1';
        wait for 1 us;
        start <= '0';
        reset <= '0';
        wait;
    end process;
end architecture;