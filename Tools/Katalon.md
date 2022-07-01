# Katalon
自动化测试工具

官方网站：https://www.katalon.com/  
文档：https://docs.katalon.com/katalon-studio/docs/index.html  
软件介绍：https://www.pianshen.com/article/39992023009/  
功能介绍：https://cloud.tencent.com/developer/article/1457742


### 相关产品
1. Katalon Studio：跨平台的测试迭代集成环境，开源免费  
2. Katalon Studio Enterprise：商用版本
3. Katalon Runtime Engine：命令行工具
4. Katalon TestOps：一个相当丰富的做测试分析、测试报告平台
5. Katalon Recorder：Katalon轻量级浏览器插件
6. Katalium：开源的基于TestNG+Selenium的轻量级自动化测试框架。(需要继续使用TestNG和Selenium的人员可以使用此框架)


### Katalon Studio 使用教程
1. 集成环境功能介绍： https://www.cnblogs.com/smile-tester/p/12220706.html  
2. 下载安装 & 创建项目：https://www.pianshen.com/article/39992023009/  
3. 项目管理相关介绍： https://cloud.tencent.com/developer/article/1523919  
4. Web录制回放功能： https://cloud.tencent.com/developer/article/1523926?from=article.detail.1524098  
5. Spy Web（1）： https://cloud.tencent.com/developer/article/1524127?from=article.detail.1524098  
6. Spy Web（2）： https://blog.csdn.net/qq_34243930/article/details/88932689  
7. 数据驱动测试： https://blog.csdn.net/Hannah2018/article/details/81634601
8. 测试报告： https://blog.csdn.net/ttphoon/article/details/91555145  
9. 检查点： https://blog.csdn.net/qq_18659137/article/details/85864582
10. 断言语句： https://blog.csdn.net/qq_18659137/article/details/85860703
11. Android App环境搭建： https://www.pianshen.com/article/9211167782/  
12. API测试： https://cloud.tencent.com/developer/article/1553016?from=article.detail.1524098  
13. 连接数据库： https://cloud.tencent.com/developer/article/1523932  
14. 关键字调用： https://cloud.tencent.com/developer/article/1523933
15. Katalon配置MySQL：https://docs.katalon.com/katalon-studio/how-to-guides/how-to-implement-ddt-mysql.html#add-an-external-jdbc-driver-for-mysql-database-connection
16. Katalon创建数据库Keyword：https://docs.katalon.com/katalon-studio/docs/handling-databases.html
17. 全局变量：http://blog.sina.com.cn/s/blog_44d19b500102yatu.html
18. Keyword定义与使用：https://www.jianshu.com/p/f9b871c2e966?utm_campaign=maleskine&utm_content=note&utm_medium=seo_notes&utm_source=recommendation
19. 获取页面元素的值：id = WebUI.getAttribute(findTestObject('Object Repository/OA添加用户/Page_Add_User/v_input_userid'), 'value')

### 故障排除
1. 安装的时候不能有中文路径  
2. https://blog.csdn.net/qq_18659137/article/details/85339482
3. 获取页面元素的值的时候，input需要用WebUI.getAttribute，而div、span中的文字需要用WebUI.getText
4. 页面点击保存后，直接下一条脚本语句查数据库记录，可能没有来得及存到数据库。可以先等待2s
5. Xpath定位页面元素的时候，有时候每次页面打开元素id是变的，此时，需要去掉元素id，然后再选择xpath。
