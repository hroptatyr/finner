%%{
machine finner;

unit_1 = 
	(num " ")? "million" @{r("num(*1000000)")} |
	num ("mm" | "m") @{r("num(*1000000)")} |
	(num " ")? "billion" @{r("num(*1000000000)")} |
	num "bn" @{r("num(*1000000000)")} |
	(num " ")? "trillion" @{r("num(*1000000000000)")} |
	num "tr" @{r("num(*1000000000000)")} |
	(num " ")? "percent" @{r("num(*0.01)")} |
	(num " "?)? "pct" @{r("num(*0.01)")} |
	num " "? "%" @{r("num(*0.01)")} |
	(num " ")? "basis points" @{r("num(*0.0001)")} |
	(num " "?)? "bps" @{r("num(*0.0001)")} |
	num " "? "bps" @{r("num(*0.0001)")} |
	zlen ;

}%%
