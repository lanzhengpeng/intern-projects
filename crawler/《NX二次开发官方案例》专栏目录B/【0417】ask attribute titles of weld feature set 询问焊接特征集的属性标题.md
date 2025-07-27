### 【0417】ask attribute titles of weld feature set 询问焊接特征集的属性标题

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_weld.h>  
    #include <uf_ui.h>  
    #include <stdarg.h>  
    /* First tested this example in NX7.5.2 译:在NX7.5.2中首次测试了这个示例。 */  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_FNT(X) report_feature_name_and_type(#X, X)  
    static void report_feature_name_and_type(char *which, tag_t feature)  
    {  
        char  
            *name,  
            *type,  
            msg[MAX_LINE_LENGTH+1];  
        if (!UF_CALL(UF_MODL_ask_feat_name(feature, &name)) &&  
            !UF_CALL(UF_MODL_ask_feat_type(feature, &type)))  
        {  
            sprintf(msg, "%s - tag_t %d - %s (%s)\n", which, feature, name, type);  
            WRITE(msg);  
            UF_free(name);  
            UF_free(type);  
        }  
    }  
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    #define WRITENZ(X) if (X != 0) ECHO("%s = %d\n", #X, X);  
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
    }  
    static void do_it(void)  
    {  
        int   
            inx=0,  
            attrCount = 0;  
        tag_t  
            feat=NULL_TAG;  
        char  
            ** attrTitle;  
        if ((feat = select_a_feature("Select weld feature set ")) != NULL_TAG)  
        {  
            WRITE_FNT( feat );  
            UF_CALL( UF_WELD_populate_attr_list( feat, &attrTitle, &attrCount ));  
            WRITE_D( attrCount );  
            for(inx = 0; inx < attrCount; inx++)  
            {  
                WRITE( attrTitle[inx] );  
                WRITE("\n");  
            }  
            UF_free_string_array(attrCount, attrTitle);  
        }  
    }  
    /* 里海 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX的二次开发示例，主要功能是选择一个焊接特征集，并获取其所有属性的名称。具体步骤如下：
>
> 1. 初始化和终止：首先调用UF_initialize()和UF_terminate()进行初始化和终止。
> 2. 选择焊接特征集：通过UF_UI_select_feature()让用户选择一个焊接特征集。
> 3. 获取属性名称：使用UF_WELD_populate_attr_list()获取特征集的所有属性名称，并输出。
> 4. 错误处理：通过UF_CALL()宏来调用NX的函数，并捕获错误码，如果出错则输出错误信息。
> 5. 辅助宏：定义了几个辅助宏，如ECHO()用于输出信息，WRITE_D()用于输出数字，WRITE_FNT()用于输出特征名称和类型等。
> 6. 卸载：通过ufusr_ask_unload()返回卸载模式。
>
> 总的来说，这段代码是一个简单的NX二次开发示例，展示了如何选择特征集、获取属性列表并输出，并包含了基本的错误处理和宏定义。
>
