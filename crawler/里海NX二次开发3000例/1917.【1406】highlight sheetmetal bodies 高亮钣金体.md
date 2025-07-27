### 【1406】highlight sheetmetal bodies 高亮钣金体

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_obj.h>  
    #include <uf_ui.h>  
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
    static tag_t ask_next_feature(tag_t part, tag_t feat)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE) return feat;  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            inx = 0,  
            count = 0;  
        uf_list_p_t  
            sheet_metal_bodies = NULL;  
        char  
            *sysname = NULL;  
        tag_t  
            thisFeat = NULL_TAG,  
            thisBody = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_MODL_create_list(&sheet_metal_bodies));  
        do  
        {  
            thisFeat = ask_next_feature(part, thisFeat);  
            if(NULL_TAG == thisFeat) break;  
            UF_CALL(UF_MODL_ask_feat_sysname( thisFeat, &sysname ));  
            if(strstr(sysname, "SB Tab") != NULL ||  
                strstr(sysname, "SB Convert") != NULL)  
            {  
                UF_CALL(UF_MODL_ask_feat_body(thisFeat, &thisBody));  
                UF_CALL(UF_MODL_put_list_item(sheet_metal_bodies, thisBody));  
            }  
        } while (thisFeat != NULL_TAG);  
        UF_CALL(UF_MODL_ask_list_count(sheet_metal_bodies, &count));  
        if(count)  
        {  
            for(inx = 0; inx < count; inx++)  
            {  
                UF_CALL(UF_MODL_ask_list_item(sheet_metal_bodies, inx, &thisBody));  
                UF_CALL(UF_DISP_set_highlight(thisBody, 1));  
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

> 这段代码是一个NX二次开发程序，其主要功能是遍历当前打开的零件，查找名称中包含“SB Tab”或“SB Convert”的特征，并高亮显示这些特征所对应的实体。
>
> 主要逻辑如下：
>
> 1. 初始化NX环境，创建一个实体列表。
> 2. 遍历当前零件中的所有特征，并检查其名称是否包含“SB Tab”或“SB Convert”。
> 3. 如果名称匹配，则获取该特征对应的实体，并将实体添加到列表中。
> 4. 遍历完成后，获取列表中的实体数量。
> 5. 如果列表中有实体，则遍历列表，对每个实体设置高亮显示。
> 6. 最后，关闭NX环境。
>
> 这段代码通过NX提供的API，实现了查找特定名称的特征并高亮显示其对应实体的功能，展示了NX二次开发的基本流程和常用API的调用方法。
>
