### 【1500】list all cone parameters 列出所有锥形参数

#### 代码

```cpp
    /*HEAD LIST_ALL_CONE_PARAMETERS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <time.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n*** ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是在V18版本中新增的。 */  
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
    static tag_t ask_next_feature(tag_t part, tag_t feat)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat));  
        return feat;  
    }  
    static void do_it(void)  
    {  
        int  
            edit = 0; /* ignored flag 里海译:根据文档，"ignored flag"指的是在标志位或参数中，被忽略的标志位。 */  
        tag_t  
            feat = NULL_TAG,  
            part = UF_PART_ask_display_part();    
        char  
            msg[MAX_LINE_SIZE+1],  
            *name,  
            *type,  
            *base_dia,  
            *top_dia,  
            *height,  
            *half_angle;  
        logical  
            found_feat = FALSE,  
            found_cone = FALSE;  
        while ((feat = ask_next_feature(part, feat)) != NULL_TAG)  
        {  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE)  
            {  
                found_feat = TRUE;  
                UF_CALL(UF_MODL_ask_feat_name(feat, &name));  
                UF_CALL(UF_MODL_ask_feat_type(feat, &type));  
                if(!strcmp(type, "CONE"))  
                {    
                    found_cone = TRUE;  
                    UF_CALL(UF_UI_open_listing_window());  
                    sprintf(msg, "%d %s (%s)\n", feat, name, type);  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                    UF_CALL(UF_MODL_ask_cone_parms(feat, edit, &base_dia,  
                        &top_dia, &height, &half_angle));  
                    sprintf(msg,"base:\t%s\ntop:\t%s\nhgt:\t%s\nhalf angle:\t%s\n",  
                         base_dia, top_dia, height, half_angle);  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
        }  
        if(found_feat)  
        {  
            UF_free(name);  
            UF_free(type);  
        }  
        if(found_cone)  
        {  
            UF_free(base_dia);  
            UF_free(top_dia);  
            UF_free(height);  
            UF_free(half_angle);  
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

> 这段代码是用于NX二次开发，其主要功能是遍历当前显示部件中的所有特征，并筛选出圆锥特征，然后输出其参数信息。
>
> 具体来说，代码包含以下关键部分：
>
> 1. 引入了NX API所需的头文件，并定义了一个错误处理函数report_error，用于输出错误信息。
> 2. 定义了一个函数ask_next_feature，用于遍历部件中的特征，并返回下一个特征。
> 3. 主函数do_it负责遍历所有特征，判断特征类型，并针对圆锥特征输出其名称和参数信息。
> 4. ufusr函数是NX二次开发的入口函数，负责初始化NX API，调用主函数，然后终止NX API。
> 5. ufusr_ask_unload函数用于卸载二次开发模块。
>
> 总体来说，这段代码实现了遍历当前显示部件中的圆锥特征，并输出其参数信息的功能。它通过NX API提供的接口实现了对NX模型的访问和操作。
>
