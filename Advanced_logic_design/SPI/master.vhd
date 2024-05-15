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
    reset : in std_logic;
    -- Command: load the data to be sent
    load_data : in std_logic;
    -- Data: the data to be sent
    data : in std_logic_vector(7 downto 0);
    -- Data the be sent currently in shift register
    data_tx : out std_logic_vector( 7 downto 0 );
    -- Data received
    data_rx : out std_logic_vector( 7 downto 0 );
    -- Command: start data transmission
    start : in std_logic;
    -- SPI interface signals (Master)
    MOSI : out std_logic;
    MISO : in std_logic;
    SCK  : out std_logic;
    SS   : out std_logic
  );
end entity spi_master;

architecture rtl of spi_master is
  type t_State is (Sending, Receiving, Idle);
  signal state : t_State := Idle;
  signal shift_enable_tx: std_logic := '0';
  signal shift_enable_rx: std_logic := '0';
  signal bit_count_en: std_logic := '0';
  signal bit_count_tc: std_logic := '0';
  signal clk_count_en: std_logic := '0';
  signal clk_count_tc: std_logic := '0';
  signal data_tx_reg: std_logic := '0';
begin
  register_tx : entity work.shift_reg
  port map
  (
    clk => SCK,
    reset => reset,
    shift_enable => shift_enable_tx,
    load_enable => load_data,
    parallel_in => data,
    data => data_tx,
    data_in => '0',
    data_out => data_tx_reg
  );
  register_rx : entity work.shift_reg
  port map
  (
    clk => SCK,
    reset => reset,
    shift_enable => shift_enable_rx,
    load_enable => '0',
    parallel_in => (others => '0') ,
    data => data_rx,
    data_in => MISO,
    data_out => open
  );
  bit_counter: entity work.counter
  generic map (
    counter_size => 3
  )
  port map (
    clk => SCK,
    reset => reset,
    count_enable => bit_count_en,
    tc => bit_count_tc
  );
  clk_counter: entity work.counter
  generic map (
    counter_size => 4
  )
  port map (
    clk => clk,
    reset => reset,
    count_enable => clk_count_en,
    tc => clk_count_tc
  );

  process ( clk, reset )
  begin
    clk_count_en <= '1';
    
    if reset = '1' then
      shift_enable_tx <= '0';
      shift_enable_rx <= '0';
      SCK <= '0';
      SS <= '0';
      MOSI <= 'Z';
    else
      if rising_edge (clk) then
        if clk_count_tc = '1' then
          SCK <= not(SCK);
        end if;
        case state is
          when Idle =>
            if clk_count_tc = '1' and start = '1' then
              bit_count_en <= '1';
              shift_enable_tx <= '1';
              state <= Sending;
            end if;
          when Sending =>
            MOSI <= data_tx_reg;
            if bit_count_tc = '1' then
              shift_enable_tx <= '0';
              -- bit_count_en <= '0';
              MOSI <= 'Z';
              shift_enable_rx <= '1';
              state <= Receiving;
            end if;
          when Receiving =>
            if bit_count_tc = '1' then
              shift_enable_rx <= '0';
              bit_count_en <= '0';
              state <= Idle;
            end if;
        end case;
      end if;
    end if;
  end process;
end architecture;