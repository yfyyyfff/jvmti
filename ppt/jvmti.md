title: JVMTI简介与实践
speaker: 杨帆
url: 
transition: cards
theme:moon
files: /js/demo.js,/css/demo.css,/js/zoom.js
highlightStyle: agate
usemathjax: yes

[slide data-transition="move"]

# JVMTI简介与实践</br></br>
### 杨帆
### 2017年10月 

[slide data-transition="move"]

## 目录

1. JVMTI简介
2. Agent简介
3. JNI基础
4. 运行加密的class文件
5. 实现类似ParamStack的功能
6. 强制方法提前返回

[slide data-transition="stick"]

## JVMTI简介
<div align="left"><font size=5>
<p>&emsp;&emsp;JVMTI(JVM Tool Interface)是由Java虚拟机提供的native编程接口，是JVMPI(Java Virtual Machine Profiler Interface)和 JVMDI(Java Virtual Machine Debug Interface)的更新版本。</p>

<p>&emsp;&emsp;JVMTI提供了调试(debug)和分析(profiler)功能；同时，它还有监听(Monitoring)，线程分析(Thread analysis)以及覆盖率分析(Coverage Analysis)等功能。</p>

<p>&emsp;&emsp;JVMTI是一套本地代码接口，因此我们需要使用C/C++以及JNI。开发时一般采用建立一个Agent(通过C++编写)的方式来使用JVMTI，它可以使用JVMTI函数、设置回调函数、从JVM中得到当前的运行态信息，还可以操作虚拟机的运行态。</p>
</font></div>

[slide data-transition="zoomout"]
## JVMTI基本功能
* 管理内存 (非JVM管理的内存)
* 获取线程和线程组信息
* 提前返回 Force Early Return
* 获取线程的栈帧信息
* 管理堆
* 管理局部变量(栈)
* 管理断点
* 管理类、对象、成员和方法
* 管理JVM事件
* ...

[slide data-transition="kontext"]
## Agent简介
----
<br>
<div align="left"><font size=5>
<p>&emsp;&emsp;一般采用Agent的方式使用JVMTI。Agent中使用JVMTI函数，设置一些回调函数，并从Java虚拟机中得到当前的运行态信息，作出自己的判断，最后还可能操作虚拟机的运行态。

<p>&emsp;&emsp;把 Agent 编译成一个动态链接库之后，我们可以在Java程序启动的时候来加载它(启动加载模式)，也可以使用运行时加载(活动加载模式)。
</font></div>
![jvmti](/img/jvmti.jpg)

[slide data-transition="zoomin"]
## 常见的agent
* JDWP
<font size=4>
<div align=left><p>JDWP是Java Debug Wire Protocol 的缩写，它定义了调试器（debugger）和被调试的 Java 虚拟机（target vm）之间的通信协议。Debugger和 target vm分别在各自的进程中运行，他们之间的通信协议就是JDWP.
</div></font>
* instrument
<font size=4>
<div align=left><p>instrument为java agent提供基础,主要作用是在ClassFileLoadHook回调函数中设计了一套框架,这套框架支持使用java编写agent。
</div></font>
[slide data-transition="cards"]
## JDWP
![jvmti](/img/jdwp.jpg)

[slide data-transition="cards"]
## Java Agent
![java agent](/img/java_agent.png)
[slide data-transition="cards"]

##  使用Agent
----
<br>
* 启动加载
<div align="left"><font face="consolas" color=#FFF0F5>
<p>java -agentpath:path/to/agent<font color=00FF00>=&lt;options&gt;</font> package/Class
<p>java -agentlib:agent<font color=00FF00>=&lt;options&gt;</font> package/Class</font></div>
* 活动加载
<pre><code>
	//jvmPID为Java虚拟机进程
	VirtualMachine virtualMachine = VirtualMachine.attach(jvmPID);
	// libPath为共享库路径，options为传递给agent的参数
	virtualMachine.loadAgentPath(libPath, options);
	virtualMachine.detach();
</code></pre>

[slide data-transition="zoomout"]

## 编写Agent
[subslide]
实现Agent_OnLoad, Agent_OnAttach和Agent_OnUnload
<pre><code>
	jvmtiEnv *jvmti;
	JNIEXPORT jint JNICALL Agent_OnLoad(JavaVM *vm, char *options, void *reserved) {
    	jvm->GetEnv((void**)&jvmti, JVMTI_VERSION_1_0);
    	return JNI_OK;
    }
    JNIEXPORT jint JNICALL Agent_OnAttach(JavaVM* vm, char* options, void* reserved) {
    	std::cout << "Agent_OnAttach" << std::endl;
    	return JNI_OK;
	}
	JNIEXPORT jint JNICALL Agent_OnUnload(JavaVM* vm, char* options, void* reserved) {
    	std::cout << "Agent_OnUnload" << std::endl;
    	return JNI_OK;
	}
</code></pre>

====
设置jvmtiCapabilities
<pre><code>
	jvmtiCapabilities cap;
	//需要什么功能就使能对应的成员
	cap.can_get_constant_pool = 1; 
	cap.can_get_bytecodes = 1; 
    cap.can_get_owned_monitor_info = 1; 
    cap.can_generate_method_entry_events = 1;
    cap.can_generate_exception_events = 1;
    cap.can_generate_vm_object_alloc_events = 1;
    cap.can_access_local_variables = 1;
    cap.can_force_early_return = 1;
</code></pre>
=====

编写关注的JVM事件的回调函数
<pre><code>
	void  classFileLoadHook
		(jvmtiEnv *jvmti_env,
	    JNIEnv* jni_env,
	    jclass class_being_redefined,
	    jobject loader,
	    const char* name,
	    jobject protection_domain,
	    jint class_data_len,
	    const unsigned char* class_data,
	    jint* new_class_data_len,
	    unsigned char** new_class_data) {
	    jvmti_env->Allocate(class_data_len, new_class_data);
	    *new_class_data_len = class_data_len;
	    if (strcmp(name, "encrypt/TestClass") != 0) {
	        memcpy(*new_class_data, class_data, class_data_len);
	    } else {
	        int i;unsigned char tmp = 0;
	        for (i = 0; i < class_data_len; i++) {
	            tmp = (class_data[i]);
	            (*new_class_data)[i] = (unsigned char)(~tmp);
	        }
    	}
	}
</code></pre>
=====

注册回调函数和JVM事件
<pre><code>
	//回调函数结构体,
	jvmtiEventCallbacks eventCallBakcs;
	//注册回调函数
	eventCallBakcs.ClassFileLoadHook = (jvmtiEventClassFileLoadHook)classFileLoadHook;
	eventCallBakcs.Exception = (jvmtiEventException)exceptionOccur;
	//注册事件
	jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_EXCEPTION, NULL);
	jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_CLASS_FILE_LOAD_HOOK, 0);
</code></pre>
[/subslide]

[slide data-transition="cards"]

## JNI基础
*  JNI(Java Native Interface)是Java和本地语言(C/C++等)交互的接口。通过该接口，Java和本地语言可以相互调用。通过JNI可以使用本地语言实现系统底层的功能供Java使用，如Thread类和Unsafe类。
*  JNI在本地代码中调用Java的元素时，都是通过类似反射的方式执行的。
*  JNI提供的函数在jni.h中声明。
*  JNI另一要素是Signature(签名)，类，方法和成员都有签名。签名表示了一个元素的结构和特性。因为重载和泛型机制，仅依赖名字无法分辨2个元素是否同一个，如StringBuffer的多个append方法和List的泛型。

[slide data-transition="cards"]
## Signature
<div align="left"><font size=4>在java home下include文件夹下可以找到classfile_constants.h,里面定义了一些常量,Signature部分如下:</font></div>
<pre><code>
	/* Type signatures */
	enum {
	    JVM_SIGNATURE_ARRAY         = '[', //左方括号
	    JVM_SIGNATURE_BYTE          = 'B',
	    JVM_SIGNATURE_CHAR          = 'C',
	    JVM_SIGNATURE_CLASS         = 'L',
	    JVM_SIGNATURE_ENDCLASS      = ';',
	    JVM_SIGNATURE_ENUM          = 'E',
	    JVM_SIGNATURE_FLOAT         = 'F',
	    JVM_SIGNATURE_DOUBLE        = 'D',
	    JVM_SIGNATURE_FUNC          = '(',
	    JVM_SIGNATURE_ENDFUNC       = ')',
	    JVM_SIGNATURE_INT           = 'I',
	    JVM_SIGNATURE_LONG          = 'J',
	    JVM_SIGNATURE_SHORT         = 'S',
	    JVM_SIGNATURE_VOID          = 'V',
	    JVM_SIGNATURE_BOOLEAN       = 'Z'
	};
</code></pre>

[slide data-transition="cards"]
## 签名示例
<table >
  <tr>
    <th width=400>元素</th>
    <th width=240>名称</th>
    <th width=750>签名</th>
  </tr>
  <tr>
    <td>public Integer(int i);</td>
    <td>&lt;init&gt;</td>
    <td>(I)V</td>
  </tr>
  <tr>
    <td>public String(char[] value)</td>
    <td>&lt;init&gt;</td>
    <td>([C)V</td>
  </tr>
  <tr>
    <td>Object.equals(Object o);</td>
    <td>equals</td>
    <td>(Ljava/lang/Object;)Z</td>
  </tr>
  <tr>
    <td>List&lt;Integer&gt; myList;</td>
    <td>myList</td>
    <td>签名Ljava/util/List;<br>泛型签名Ljava/util/List&lt;Ljava/lang/Integer;&gt;;</td>
  </tr>
  <tr>
    <td>StringBuffer.append(int i);</td>
    <td>append</td>
    <td>(I)Ljava/lang/StringBuffer;</td>
  </tr>
  <tr>
    <td>StringBuffer.replace(int start,int end,String s);</td>
    <td>replace</td>
    <td>(IILjava/lang/String;)Ljava/lang/StringBuffer;</td>
  </tr>
</table>

[slide data-transition="cards"]

## demo演示
[slide data-transition="cards"]
## 运行加密的class文件
<br>
<div align="left">
<p>加密的class文件无法被jvm加载,需要解密为正确的字节码
</div>

[slide data-transition="cards"]
## 实现类似stackparam的功能
<br>
<div align="left">
<p>stackparam是一个可以获取调用栈上每个函数参数的工具。现在实现一个当触发异常时,打印调用栈上每个方法局部变量值的功能。
</div>
[slide data-transition="cards"]
## 强制方法提前返回
<br>
* 解决死循环
* 测试时对该函数打桩
* 在不修改字节码的情况下修改功能
