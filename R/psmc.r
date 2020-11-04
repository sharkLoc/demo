library(ggplot2)
library(scales)

d1<-read.table("psmc.txt")
d0<-read.table("psmc0.txt")

ggplot()+
  geom_step(d1,mapping = aes(x=V1,y=V2),colour="red",lwd=1.2)+
  geom_step(d0,mapping = aes(x=V1,y=V2),colour="green",lwd=1.2)+
  labs(x=expression("Years (g=0.4, µ=0.2x"*10^-8*")"),
       y = expression("Effective population size ( x"~10^4*")")
	   )+
  scale_x_log10(breaks = trans_breaks("log10", function(x) 10^x),
               labels = trans_format("log10", math_format(10^.x))
			   )+
  annotation_logticks(sides = "bottom")+
  #scale_x_log10()+
  #scale_x_continuous(limits=c(10*4,10**7))+
  theme(
        panel.grid.minor=element_blank(),
        plot.title = element_text(hjust = 0.5,size = 20),
        axis.text.x = element_text(size = 15),
        axis.text.y = element_text(size = 15),
        axis.title = element_text(size = 15),
        panel.grid.major=element_line(colour='grey',linetype="dashed"),
        panel.background = element_rect(fill = "white",colour='black')
        )
