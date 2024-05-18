-- Library declarations
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
-- A generic Master SPI interface
entity spi_slave is
  generic (
    -- Number of bits to transfer
    NUM  : natural   := 8;
    CPHA : std_logic := '1'
  );
  port (
    -- System clock
    clk : in std_logic;
    -- System reset (active low)
    reset : in std_logic;
    -- Command: load the data to be sent
    load_data : in std_logic;
    -- Data: the data to be sent
    data : in std_logic_vector(7 downto 0);
    -- Data the be sent currently in shift register
    data_tx : inout std_logic_vector( 7 downto 0 );
    -- Data received
    data_rx : inout std_logic_vector( 7 downto 0 );
    -- SPI interface signals (Master)
    MOSI : in std_logic;
    MISO : out std_logic;
    SCK  : in std_logic;
    SS   : in std_logic
  );
end entity spi_slave;

architecture rtl of spi_slave is
  type t_State is (Idle, Transmitting, Receiving);
  signal shift_tx: std_logic := '0';
  signal shift_rx: std_logic := '0';
  signal data_out_tx: std_logic := '0';
  signal state : t_State := Idle;
  signal prev_clk : std_logic := '0';
begin
  register_tx : entity work.shift_reg
  port map (
    clk => clk,
    reset => reset,
    shift_enable => shift_tx,
    chip_enable => SS,
    load_enable => load_data,
    parallel_in => data,
    data => data_tx,
    data_in => '0',
    data_out => data_out_tx
  );
  register_rx : entity work.shift_reg
  port map (
    clk => clk,
    reset => reset,
    shift_enable => shift_rx,
    chip_enable => SS,
    load_enable => '0',
    parallel_in => (others => '0') ,
    data => data_rx,
    data_in => MOSI,
    data_out => open
  );

  MISO <= data_out_tx when SS = '0' else 'Z';

  process ( clk, reset )
  begin    
    if reset = '0' then
      shift_rx <= '0';
      shift_tx <= '0';
      prev_clk <= '0';
      state <= Idle;
    else
      if rising_edge (clk) then
        if state = Idle and SS = '0' then
          prev_clk <= SCK;
          if CPHA = '0' then
            shift_tx <= '1';
            state <= Transmitting;
          else
            state <= Receiving;
          end if;
        elsif state = Transmitting then
          shift_tx <= '0';
          if SCK /= prev_clk then
            prev_clk <= SCK;
            shift_rx <= '1';
            state <= Receiving;
          end if;
        elsif state = Receiving then
          shift_rx <= '0';
          if SCK /= prev_clk then
            prev_clk <= SCK;
            shift_tx <= '1';
            state <= Transmitting;
          end if;
        end if;
        if state /= Idle and SS = '1' then
          state <= Idle;
        end if;
      end if;
    end if;
  end process;
end architecture;