### 【1386】highlight manual dimensions 高亮手动尺寸

#### 代码

```cpp
    /*HEAD HIGHLIGHT_MANUAL_DIMENSIONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_drf.h>  
    #include <uf_disp.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，翻译如下：

注：UF_print_syslog 是在 V18 版本中新增的。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
    static tag_t ask_next_dim(tag_t part, tag_t dim)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_dimension_type, &dim))  
            && (dim != NULL_TAG))  
        {  
            if (!UF_ASSEM_is_occurrence(dim)) return dim;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            mpi[100];  
        double  
            mpr[70];  
        char  
            rad[27],  
            dia[27];  
        tag_t  
            callout,  
            dim = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        while ((dim = ask_next_dim(part, dim)) != NULL_TAG)  
        {  
            UF_CALL(UF_DRF_ask_object_preferences(dim, mpi, mpr, rad, dia));  
            if ((mpi[7] == 3) || (mpi[7] == 4))  
            {  
            /*  callouts created using Insert->Model Parameters->Feature...  
                contain manual dimensions which are "OK" - see PR 4683200 */  
                UF_DRF_ask_callout_of_annotation(dim, &callout);  
                if ((callout != NULL_TAG)) continue;  
                UF_CALL(UF_DISP_set_highlight(dim, 1));  
                uc1601("highlighted dimension has manual text", 1);  
                UF_CALL(UF_DISP_set_highlight(dim, 0));  
            }  
        }  
    }  
    /* qq3123197280 */  
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

> 这段代码是用于NX软件的二次开发代码，其主要功能是高亮显示包含手动文本的尺寸标注。具体来说：
>
> 1. 包含了NX Open的常见头文件，用于访问NX的几何对象、显示、注释等API。
> 2. 定义了report_error函数，用于打印错误信息。
> 3. ask_next_dim函数用于遍历零件中的所有尺寸标注对象，并返回下一个尺寸标注的tag。
> 4. do_it函数是主函数，首先获取当前显示的零件，然后遍历零件中的所有尺寸标注，对于每个尺寸标注：
>
> 1. ufusr是NX调用的入口函数，其中初始化NX环境，调用do_it函数，然后终止NX环境。
> 2. ufusr_ask_unload函数用于卸载用户自定义函数，这里直接返回UF_UNLOAD_IMMEDIATELY表示立即卸载。
>
> 综上所述，这段代码的主要作用是高亮显示包含手动文本的尺寸标注，以便进行识别和修改。
>
