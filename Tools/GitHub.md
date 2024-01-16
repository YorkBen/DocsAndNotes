## 学习资料
1. https://www.w3schools.com/git/git_ignore.asp?remote=github

## windows github 命令行使用
1. 下载git客户端工具，以下2个网站一样的
https://git-for-windows.github.io/
https://git-scm.com/download/win/

2. 初始化
ssh-keygen -t rsa -C "weiqi5@qq.com" 有输入的全部回车

3. 网站配置
转到github网站上去配置一下ssh key,点击箭头指示的三角图标，选择Settings，然后点击左侧的SSH Keys，
之后点击右侧的Add SSH Key，这样就会出现添加SSH Key的界面，在Title这一栏填一个名字，名字随意起，
之后打开刚才生成的那个文件id_rsa.pub，全选复制里面的内容到Key这一栏中，点击Add Key按钮完成操作，
这时你填的邮箱会收到一封确认的邮件，不用管它

4. 命令行设置
ssh -T git@github.com 会提示success，第一次需要输入yes
git config --global user.name "wjx0912"
git config --global user.email "weiqi5@qq.com"

5. 之后的操作
git clone git@github.com:wjx0912/CppBaseUtils.git
修改代码
git status .
git add .
git commit . -m "comment string"
git push origin master


## github问题解决
1. 不小心向GitHub提交大文件，不能push也无法撤销删除。
    git filter-branch -f --index-filter 'git rm --cached --ignore-unmatch system_status/test_26457'
  又出现问题：Cannot rewrite branches: You have unstaged changes.
    git stash暴力解决。
    然后再执行第一条命令，再push就可以了。

2. git提交了大文件，无法回撤。
   因为在push之前已经进行了commit操作，所以可以尝试将文件备份后，再回退到之前的commit版本，push一下，再将备份的文件进行处理，再复制到对应地址，重新add、commit、push，涉及的指令：
   查看所有提交的版本信息： git reflog
   版本回退：git reset --hard commit_id

3. 使用git clone时提示需要输入账户密码，但是github目前已经不允许使用密码直接登录，而是要使用personal access token. 参考：https://docs.github.com/en/enterprise-server@3.8/authentication/keeping-your-account-and-data-secure/managing-your-personal-access-tokens

4. 修改了README.md导致提交失败
    a. 先提交本地版本：git commit -a -m "提交本地版本"
    b. git fetch origin
    c. git merge -m "解决与上游版本不一致问题" origin/master
    d. git push origin master

5. Your branch and 'origin/main' have diverged, and have 1 and 1 different commits each, respectively
参考：https://poanchen.github.io/blog/2020/09/19/what-to-do-when-git-branch-has-diverged
