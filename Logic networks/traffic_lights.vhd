library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity traffic_lights is
    Port ( clock : in std_logic;
        reset: in std_logic;
        light_1 : out std_logic_vector(0 to 2); --! Traffic light 1 red-amber-green
        light_2 : out std_logic_vector(0 to 2) --! Traffic light 2 red-amber-green
    );
end traffic_lights;

architecture Behavioral of traffic_lights is
    type State_Type is (S1, S2, S3, S4, S5, S6, S7, S8);
    signal CurrentState : State_Type := S8;
begin
    clk_and_reset: process(reset, clock)
    begin
        if(reset = '1') then
            CurrentState <= S8;
        elsif(rising_edge(clock)) then
            case CurrentState is
                when S1 => CurrentState <= S2;
                when S2 => CurrentState <= S3;
                when S3 => CurrentState <= S4;
                when S4 => CurrentState <= S5;
                when S5 => CurrentState <= S6;
                when S6 => CurrentState <= S7;
                when S7 => CurrentState <= S8;
                when S8 => CurrentState <= S1;
            end case;
        end if;
    end process;

    outputfunction: process(CurrentState)
    begin
        case CurrentState is
            when S1 =>  
                light_1 <= "100";
                light_2 <= "110";
            when S2 =>
                light_1 <= "100";
                light_2 <= "001";
            when S3 =>
                light_1 <= "100";
                light_2 <= "010";
            when S4 =>
                light_1 <= "100";
                light_2 <= "100";
            when S5 =>
                light_1 <= "110";
                light_2 <= "100";
            when S6 =>
                light_1 <= "101";
                light_2 <= "100";
            when S7 =>
                light_1 <= "010";
                light_2 <= "100";
            when S8 =>
                light_1 <= "100";
                light_2 <= "100";
        end case;
    end process;
end Behavioral;
