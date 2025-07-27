### 【1888】report all annotation text 报告所有注释文本

#### 代码

```cpp
    /*HEAD REPORT_ALL_ANNOTATION_TEXT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_drf.h>  
    #include <uf_ui.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是在V18版本中新增的。因此，针对问题“UF_print_syslog is new in V18”，我的翻译为：“UF_print_syslog是V18版本新增的。” */  
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
    static tag_t ask_next_drafting_aid(tag_t part, tag_t draft)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drafting_entity_type, &draft));  
        return (draft);  
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
            ann_data[10],  
            ann_data_type,  
            ann_data_form,  
            num_segments,  
            cycle = 0,  
            ii,  
            length,  
            mask[4] = { 0,0,1,0 },  
            status,  
            size;  
        tag_t  
            draft = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        double  
            ann_origin[2],  
            radius_angle;  
        char  
            name[UF_OBJ_NAME_LEN + 1],  
            messg[MAX_LINE_SIZE+1],  
            text[1000];  
        UF_CALL(UF_UI_open_listing_window());  
        while ((draft = ask_next_drafting_aid(part, draft)) != NULL_TAG)  
        {  
            if (!uc6409(draft, &status, name) && (status == 1))  
                sprintf(messg, "%d - on %s", draft, name);  
            else  
                sprintf(messg, "%d - ", draft);  
            UF_UI_write_listing_window(messg);  
            while (!UF_CALL(UF_DRF_ask_ann_data(&draft, mask, &cycle, ann_data,  
                &ann_data_type, &ann_data_form, &num_segments, ann_origin,  
                &radius_angle)) && (cycle != 0))  
            {  
                sprintf(messg, "  text at %f, %f\n", ann_origin[0], ann_origin[1]);  
                UF_UI_write_listing_window(messg);  
                for (ii = 1; ii <= num_segments; ii++)  
                {  
                    UF_CALL(uc5574(ii, ann_data, text, &size, &length));  
                    sprintf(messg, "    %d. %s (%d chars)\n", ii, text, length);  
                    UF_UI_write_listing_window(messg);  
                }  
            }  
        }  
        while ((draft = ask_next_dim(part, draft)) != NULL_TAG)  
        {  
            if (!uc6409(draft, &status, name) && (status == 1))  
                sprintf(messg, "Found %d - view dependent in %s\n", draft, name);  
            else  
                sprintf(messg, "Found %d - not view dependent\n", draft);  
            UF_UI_write_listing_window(messg);  
            while (!UF_CALL(UF_DRF_ask_ann_data(&draft, mask, &cycle, ann_data,  
                &ann_data_type, &ann_data_form, &num_segments, ann_origin,  
                &radius_angle)) && (cycle != 0))  
            {  
                sprintf(messg, "  text at %f, %f\n", ann_origin[0], ann_origin[1]);  
                UF_UI_write_listing_window(messg);  
                for (ii = 1; ii <= num_segments; ii++)  
                {  
                    UF_CALL(uc5574(ii, ann_data, text, &size, &length));  
                    sprintf(messg, "    %d. %s (%d chars)\n", ii, text, length);  
                    UF_UI_write_listing_window(messg);  
                }  
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

> 这段代码是NX的二次开发代码，其主要功能是遍历当前打开的零件，并获取所有标注对象的信息。代码的关键点如下：
>
> 1. 错误报告函数：定义了report_error函数，用于报告函数调用失败时的错误信息。
> 2. 获取标注信息：定义了ask_next_drafting_aid和ask_next_dim函数，用于遍历零件中的标注对象。
> 3. 获取标注数据：使用UF_DRF_ask_ann_data函数获取标注的文本信息，包括位置、文本内容等。
> 4. 处理标注文本：使用uc5574函数处理标注的文本信息。
> 5. 遍历标注对象：在do_it函数中，先遍历所有标注对象，然后调用UF_DRF_ask_ann_data获取文本信息，并打印出来。
> 6. 主函数入口：定义了ufusr函数作为主函数入口，用于初始化NX环境，调用do_it函数，然后终止NX环境。
> 7. 卸载函数：定义了ufusr_ask_unload函数，用于在卸载时立即卸载。
>
> 总的来说，这段代码的主要功能是遍历零件中的标注对象，并获取标注的文本信息，然后打印出来。
>
