# linux
#在每行最后加两个空格，就可以实现换行  

常用命令：  
git init 新建一个空的仓库  
git status 查看状态  
git add . 添加文件  
git commit -m '注释' 提交添加的文件并备注说明  
git remote add origin git@github.com:jinzhaogit/git.git 连接远程仓库  
git push -u origin master 将本地仓库文件推送到远程仓库  
git log 查看变更日志  
git reset --hard 版本号前六位 回归到指定版本  
git branch 查看分支  
git branch newname 创建一个叫newname的分支  
git checkout newname 切换到叫newname的分支上  
git merge newname 把newname分支合并到当前分支上  
git pull origin master 将master分支上的内容拉到本地上  


git add --all/-A   
git add .  
两者都可以将工作区中所有未跟踪或者修改的文件添加到暂存区。  
但是两者还是有一些区别的，下面分别做一下介绍：  
一.版本导致的差别：  
1.x版本：  
 （1）.git add all可以提交未跟踪、修改和删除文件。  
 （2）.git add .可以提交未跟踪和修改文件，但是不处理删除文件。  
2.x版本：  
   两者功能在提交类型方面是相同的。  
二.所在目录不同导致的差异：  
 （1）.git add all无论在哪个目录执行都会提交相应文件。  
 （2）.git add .只能够提交当前目录或者它后代目录下相应文件。  
