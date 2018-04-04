%%{
machine finner;

unit_1 = 
	("million" "s"? | "mm" | "m") @{r("*1000000")} |
	("billion" "s"? | "bn") @{r("*1000000000")} |
	("trillion" "s"? | "tr") @{r("*1000000000000")} |
	("%" | "percent" | "pct") @{r("*0.01")} |
	("bp" "s"? | "basis points") @{r("*0.0001")} ;

}%%
