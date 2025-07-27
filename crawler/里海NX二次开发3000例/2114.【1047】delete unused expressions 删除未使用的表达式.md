### 【1047】delete unused expressions 删除未使用的表达式

#### 代码

```cpp
    /*****************************************************************************  
    **  
    ** delete_unused_expressions.c  
    **  
    ** This example deletes expressions in the work part that are   
    ** not being used to control any features.  
    **  
    *****************************************************************************/  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_text.h>  
    #include <uf_ui.h>  
    #include <stdarg.h>  
    #pragma warning(disable:4996)  
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
    static void do_it(void)  
    {  
        int  
            numExps = 0,  
            inx = 0,  
            jnx = 0,  
            numFeatures = 0;  
        tag_t  
            workPart = NULL_TAG,  
            *exps = NULL,  
            *feats = NULL;  
        workPart = UF_ASSEM_ask_work_part();  
        if(NULL_TAG == workPart) return;  
        UF_CALL(UF_MODL_ask_exps_of_part(workPart, &numExps, &exps));  
        for (inx = 0; inx < numExps; inx++)  
        {  
            tag_t  
                thisExp = exps[inx];  
            UF_CALL(UF_MODL_ask_features_of_exp(thisExp, &numFeatures, &feats));  
            if(numFeatures == 0)  
            {  
                UF_CALL(UF_MODL_delete_exp_tag(thisExp));  
            }  
            else  
            {  
                UF_free(feats);  
                feats = NULL;  
            }  
        }  
        if(numExps > 0)  
        {  
            UF_free(exps);  
        }  
        UF_MODL_update();  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_TEXT_set_text_mode(UF_TEXT_ALL_UTF8));  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能是删除工作部件中未用于控制任何特征的表达式。具体来说，代码的主要步骤包括：
>
> 1. 包含必要的NX API头文件。
> 2. 定义ECHO函数，用于打印日志信息。
> 3. 定义UF_CALL宏，用于调用NX API函数并捕获错误。
> 4. 定义report_error函数，用于报告API调用错误。
> 5. 定义do_it函数，实现删除未使用表达式的逻辑。
> 6. 获取当前工作部件。
> 7. 获取工作部件中的所有表达式，并遍历每个表达式。
> 8. 对于每个表达式，获取其控制的特征数量。
> 9. 如果表达式没有控制任何特征，则删除该表达式。
> 10. 更新模型数据。
> 11. 定义ufusr函数，作为NX二次开发的入口点，调用do_it函数。
> 12. 定义ufusr_ask_unload函数，在二次开发模块卸载时调用。
>
> 总体来说，这段代码通过NX的模型API，实现了删除工作部件中未使用表达式的功能，以提高模型的数据质量。代码结构清晰，使用了错误处理机制，并符合NX二次开发的一般流程。
>
