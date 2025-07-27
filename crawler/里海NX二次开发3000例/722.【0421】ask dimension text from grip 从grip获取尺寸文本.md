### 【0421】ask dimension text from grip 从grip获取尺寸文本

#### 代码

```cpp
    /*HEAD ASK_DIMENSION_TEXT_FROM_GRIP CCC UFUN */  
    /*  This program was written as a workaround to PR-4687801  
        where GRIP EDA &DMTEXT cannot Cannot read appended text   
        for dimension which has enhanced appended text locations.  
        ENTITY/ dim  
    NUMBER/ n_lines  
    STRING/ dim_text(10,132)  
    GRARGS/ dim, n_lines, dim_text  
    IDENT/ 'Select Dimension', dim, resp   
    XSPAWN/ UFUN, '/ufunc/ask_dimension_text_from_grip.sl'  
    DO/ END:, ii, 1, n_lines  
        PRINT/ dim_text(ii)  
    END:  
    HALT  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，V18版本中新增了UF_print_syslog函数。

这个函数用于在Uniface的syslog中打印信息，其原型为：

```c
void UF_print_syslog(char *fmt, ...);

```

```cpp
# 代码解析
这段代码是一个NX Open Grip用户自定义函数(UDF)，主要目的是解决PR-4687801中提到的GRIP EDA无法读取增强型附加文本位置的问题。代码的工作原理如下：

1. **参数传递**：通过UF_ask_grip_args获取调用UDF时传递的参数，包括dim(尺寸标签)、n_lines(文本行数)和dim_text(文本数组)。

2. **获取尺寸文本**：使用UF_DRF_ask_draft_aid_text_info获取尺寸文本和公差文本，并将它们保存到dim_text数组和tol_text变量中。

3. **获取附加文本**：使用UF_DRF_ask_appended_text获取尺寸的附加文本，并根据其位置保存到app_bfr、app_aft、app_abv和app_blw变量中。

4. **设置返回参数**：将获取的文本和行数设置回调用UDF时传递的参数中，以返回给调用者。

5. **错误处理**：使用UF_CALL宏来包装UF函数调用，并在出错时打印错误信息。

6. **函数入口点**：ufusr函数是UDF的入口点，进行初始化、调用do_it处理逻辑，然后终止。

7. **卸载函数**：ufusr_ask_unload函数返回立即卸载，表示UDF可以立即被NX卸载。

总的来说，这段代码通过调用NX DRF API获取尺寸的文本信息，并作为返回值传递给调用者，解决了GRIP EDA读取附加文本的问题。
![是里海啊](https://img-blog.csdnimg.cn/4c206213ad7b4596bd2a216c390652ce.gif#pic_center)


```

#### 代码解析
> 未找到解析内容

