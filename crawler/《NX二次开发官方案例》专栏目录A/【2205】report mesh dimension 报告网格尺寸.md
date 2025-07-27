### 【2205】report mesh dimension 报告网格尺寸

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_sf.h>  
    #include <stdarg.h>  
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
    static int mask_for_meshes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_sfem_mesh_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_meshes(char *prompt, tag_t **meshes)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        tag_t  
            *objs;  
        UF_CALL(UF_UI_select_with_class_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_meshes, NULL, &resp, &cnt, meshes));  
        objs = *meshes;  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight(objs[ii], FALSE));  
        return cnt;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            cnt;  
        tag_t  
            *meshes;  
        UF_SF_mesh_dimension_t  
            dimension;  
        while ((cnt = select_meshes("Select meshes", &meshes)) > 0)  
        {  
            for (ii = 0; ii < cnt; ii++)  
            {  
            //  This fails starting in NX6 - see PR 2162105  
                if (!UF_CALL(UF_SF_ask_mesh_dimension(meshes[ii], &dimension)))  
                    ECHO("mesh %d has dimension = %d\n", ii+1, dimension);  
            }  
            UF_free(meshes);  
        }  
    }  
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

> 这段代码是用于NX的二次开发，其主要功能是让用户选择网格，并获取每个网格的维数信息。下面是代码的主要组成部分：
>
> 1. 错误报告函数：report_error()函数用于在函数调用失败时报告错误，包括错误码、错误消息和失败的函数调用。
> 2. 网格选择函数：select_meshes()函数用于打开选择对话框，让用户选择网格，并返回网格的标签数组。mask_for_meshes()函数用于设置选择对话框的过滤条件，仅显示网格类型的对象。
> 3. 获取网格维数：do_it()函数用于循环调用select_meshes()获取用户选择的网格，然后调用UF_SF_ask_mesh_dimension()函数获取每个网格的维数，并打印输出。
> 4. 主函数：ufusr()函数是二次开发程序的入口函数。在这里，首先调用UF_initialize()进行初始化，然后调用do_it()执行主要功能，最后调用UF_terminate()进行清理。
> 5. 卸载函数：ufusr_ask_unload()函数用于设置程序的卸载方式，这里设置为立即卸载。
>
> 整体而言，这段代码通过对话框让用户选择网格，然后获取并打印出每个网格的维数信息。
>
