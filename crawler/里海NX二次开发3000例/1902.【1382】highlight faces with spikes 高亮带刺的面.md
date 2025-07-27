### 【1382】highlight faces with spikes 高亮带刺的面

#### 代码

```cpp
    /*HEAD HIGHLIGHT_FACES_WITH_SPIKES CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
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

UF_print_syslog是V18版本中的新功能。 */  
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
    static tag_t ask_next_face(tag_t part, tag_t face)  
    {  
        int  
            status,  
            subtype,  
            type;  
        while ((! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &face)))  
            && (face != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(face, &type, &subtype));  
            status = UF_OBJ_ask_status(face);  
            if ((subtype == UF_solid_face_subtype) && (status == UF_OBJ_ALIVE))  
                return face;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        logical  
            has_spike;  
        int  
            cnt = 0;  
        tag_t  
            face = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            msg[133];  
        while ((face = ask_next_face(part, face)) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_face_spikes(face, &has_spike));  
            if (has_spike)  
            {  
                cnt++;  
                UF_CALL(UF_DISP_set_highlight(face, TRUE));  
            }  
        }  
        if (cnt > 0)  
        {  
            sprintf(msg, "%d faces with spikes", cnt);  
            uc1601(msg, TRUE);  
            UF_CALL(UF_PART_cleanup(UF_PART_cleanup_highlight));  
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

> 这段NX二次开发代码的主要功能是高亮显示包含尖刺的实体面。以下是代码的主要组成部分：
>
> 1. 头文件包含：引入了NX Open的常用库头文件，如uf.h、uf_ui.h等。
> 2. 错误报告函数：定义了report_error函数，用于打印出NX API调用失败的错误信息，包括错误码、错误消息等。
> 3. 获取下一个面：定义了ask_next_face函数，用于遍历实体中的所有面，并返回下一个面。
> 4. 高亮显示尖刺面：定义了do_it函数，遍历实体中的所有面，并调用UF_MODL_ask_face_spikes函数判断面是否包含尖刺，如果是，则调用UF_DISP_set_highlight函数将其高亮显示。
> 5. 主函数：定义了ufusr函数，首先调用UF_initialize初始化NX Open，然后调用do_it函数执行高亮显示操作，最后调用UF_terminate终止NX Open。
> 6. 卸载函数：定义了ufusr_ask_unload函数，用于在插件卸载时返回立即卸载的标志。
>
> 总体来说，这段代码实现了遍历实体，高亮显示包含尖刺的面的功能，并包含了错误处理、NX Open初始化/终止等常规操作。
>
