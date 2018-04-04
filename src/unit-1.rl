%%{
machine finner;

unit_1 = 
	("million" "s"? | "mm" | "m") @{state("*1000000")} |
	("billion" "s"? | "bn") @{state("*1000000000")} |
	("trillion" "s"? | "tr") @{state("*1000000000000")} |
	("%" | "percent" | "pct") @{state("*0.01")} |
	("bp" "s"? | "basis points") @{state("*0.0001")} ;

}%%
