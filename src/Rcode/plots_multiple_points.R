# plots for multiple time points
setwd("~/Documents/barcode_single_end/simulation_data")

#load estimated
timepoint = seq(1,161, by =8)
files = paste("Truth_time_point_", timepoint, "_seq.csv", sep = "")
truth = read.table(files[1], sep =',', header = T)
for( f in files[-1]) {
  temp = read.table(f, sep = ',', header = T)
  truth = cbind(truth, temp[,2])
}

colnames(truth) = c("barcode", timepoint)


# load estimated trajectory

template = truth[,1]

timepoint = seq(161, 1, by = -8)


combined_files = "combine_result_cluster.csv"

combined_result = read.csv(combined_files, header = T)

names(combined_result) = c("Cluster.ID", "barcode", "score",timepoint)

combined_result = combined_result[, c("barcode", rev(timepoint))]

overlap= merge(truth, combined_result, by = "barcode")


# pick up some representative trajectory for plotting




N = 500
set.seed(2)
index = sample(1:nrow(overlap), N, replace = F)

selected_traj_truth = as.data.frame(t(overlap[index, c(paste(rev(timepoint), 'x', sep = '.'))]))
selected_traj_est = as.data.frame(t(overlap[index, c(paste(rev(timepoint), 'y', sep = '.'))]))

selected_traj = cbind(selected_traj_truth, selected_traj_est)
selected_traj = cbind(rev(timepoint), selected_traj)

colnames(selected_traj) = c("timepoint", paste(rep(c("truth", "estimated"), each = N), rep(1:N, 2), sep = '_'))


library(ggplot2)
library(reshape2)
library(plyr)

groups = paste(rep(c("truth", "estimated"), each = N), rep(1:N, 2), sep = '_')

colors()
melted_traj <- melt(selected_traj, id = "timepoint") 
my.colors <- rep(rep(sample(colors(), N, replace = T), each = 21),2)
group = ifelse(rep(c(1,0), each = 21*N), "Truth","Estimated")
group = factor(ifelse(rep(c(1,0), each = 21*N), "Truth","Estimated"))
fill_value = rep(as.numeric(apply(selected_traj[, -1], 2, max)), each = 21)
p <- ggplot(data=melted_traj)
p <-p + geom_line(aes(x=timepoint, y=value, group = variable,linetype=group, colour = group)) 
p <- p + ggtitle("Sampled trajectories From truth and estimated")+ xlab("Time points") + ylab("Size") 
p <- p + theme(plot.title = element_text(size = 15, face = "bold"))
p <- p + theme(
  axis.title.x = element_text(color="forestgreen", vjust=-0.35,size = 20),
  axis.title.y = element_text(color="cadetblue" , vjust=0.35, size = 20),
  axis.text.x = element_text(size = 15),
  axis.text.y = element_text(size = 15))
p <- p + xlim(c(1, 162))
p <- p + theme(legend.title=element_blank())
p <- p + guides(colour = guide_legend(override.aes = list(size=5)))
p
p + scale_fill_continuous(guide = "legend")
#+ scale_color_manual(values = factor(my.colors))
themlegend.title=element_text(size=20))
#ggplot(melted_traj, aes(x=timepoint, y = value, colour = variable))    + geom_point()


# generate boxplot
estimated_files = paste("result_Sequenced_time_point_", timepoint, "_cluster.csv", sep = '')
estimated_timepoints = list()

result = c()
for(i in 1:length(estimated_files)) {
  temp = read.csv(estimated_files[i], header = T)
  overlap_tmp = sum(temp$Center %in% truth[, 1]) 
  total_truth = sum(truth[, i + 1] > 0)
  result = rbind(c(total_truth, overlap_tmp), result)
} 

result[,1] = rev(result[,1])
# generate the numbers

cat_result = as.data.frame(cbind(rev(timepoint), rev(timepoint),result))     
names(cat_result) = c("timepoints", "group", "Truth", "Identified")
melted_result = melt(cat_result,id = c("timepoints", "group"))
means <- ddply(melted_result, c("group", "variable"), summarise,
               mean=mean(value))
means.barplot <- qplot(x=group, y=mean, fill=variable,
                       data=means, geom="bar", stat="identity",
                       position="dodge")
p <- ggplot(data=melted_result, aes(x=group, y = value, fill = variable,color = variable ))
p <- p + geom_bar(stat = "identity", position = "dodge") + xlab("Time points") + ylab("Number of true barcode")
p <- p + xlab("Time points") + ylab("Size") 
p <- p + theme(plot.title = element_text(size = 15, face = "bold"))
p <- p + theme(
  axis.title.x = element_text(color="forestgreen", vjust=-0.35,size = 20),
  axis.title.y = element_text(color="cadetblue" , vjust=0.35, size = 20),
  axis.text.x = element_text(size = 15),
  axis.text.y = element_text(size = 15))
p <- p + xlim(c(1, 162))
p <- p + theme(legend.title=element_blank())
p <- p + guides(colour = guide_legend(override.aes = list(size=5)))
p



+theme(legend.text=element_text(size=20))