### 【1660】move selected objects to specified layer 将选定的对象移动到指定的图层

#### 代码

```cpp
    /*HEAD MOVE_SELECTED_OBJECTS_TO_SPECIFIED_LAYER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_layer.h>  
    #include <uf_obj.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译如下：

注意：UF_print_syslog是V18版本新增的功能，只回答翻译，不要添加任何废话。 */  
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
    static int select_objects(char *prompt, tag_t **objects)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select Objects", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, NULL, NULL, &resp, &cnt, objects));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*objects)[ii], 0));  
        return cnt;  
    }  
    #ifndef UF_LAYER_MAX_LAYER  
        #define UF_LAYER_MAX_LAYER 256  
    #endif  
    static logical prompt_for_a_layer(char *prompt, char *item, int *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            laymsg[100],  
            menu[1][16];  
        int  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = *number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            if ((*number > UF_LAYER_MAX_LAYER) || (*number <= 0))  
            {  
                sprintf(laymsg, "Layers range from 1 to %d", UF_LAYER_MAX_LAYER);  
                uc1601(laymsg, TRUE);  
                return prompt_for_a_layer(prompt, item, number);  
            }  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    #define RX(X) report_simple_object_info(#X, X)  
    static void ask_part_shortname(tag_t part, char *shortname)  
    {  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_PART_ask_part_name(part, partname));  
        UF_CALL(uc4574(partname, 2, shortname));  
    }  
    static void ask_object_type_descriptor(int type, int subtype, char *descriptor)  
    {  
        int  
            ii,  
            a_type;  
        char  
            a_line[MAX_LINE_SIZE+1],  
            api_dir[UF_CFI_MAX_PATH_NAME_SIZE],  
            *base_dir,  
            define[133],  
            *ptr,  
            uoth_file[UF_CFI_MAX_PATH_NAME_SIZE];  
        FILE  
            *in_file;  
        switch (type)  
        {  
            case UF_circle_type: /* non-std format in the header file 里海译:头文件中的非标准格式 */  
                strcpy(descriptor, "arc");  
                return;  
            case UF_OBJ_EXPR_TYPE:  
                strcpy(descriptor, "expression");  
                return;  
            case UF_OBJ_PART_TYPE:  
                strcpy(descriptor, "part");  
                return;  
            default:  
                break;  
        }  
        UF_CALL(UF_translate_variable("UGII_BASE_DIR", &base_dir));  
        UF_CALL(uc4575(base_dir, 100, "ugopen", api_dir));  
        UF_CALL(uc4575(api_dir, 0, "uf_object_types.h", uoth_file));  
        if ((in_file = fopen(uoth_file, "r")) == NULL)  
        {  
            strcpy(descriptor, "(uf_object_types.h not found)");  
            return;  
        }  
        strcpy(descriptor, "Unknown type");  /* In case its not found 里海译:如果找不到， */  
        while (!feof(in_file))  
        {  
            fgets(a_line, MAX_LINE_SIZE, in_file);  
            if ((ii = sscanf(a_line, "#define UF_%s %d", define, &a_type)) == 2)  
            {  
                if ((ptr = strstr(define, "_type")) && (a_type == type))  
                {  
                    *ptr = '\0';  
                    strcpy(descriptor, define);  
                    break;  
                }  
            }  
        }  
        while (!feof(in_file))  
        {  
            fgets(a_line, MAX_LINE_SIZE, in_file);  
            if ((ii = sscanf(a_line, "#define UF_%s %d", define, &a_type)) == 2)  
            {  
                if (ptr = strstr(define, "_type")) break;  
                if (a_type == subtype)  
                {  
                    if (ptr = strstr(define, "_subtype")) *ptr = '\0';  
                    if (strstr(define, descriptor) == define)  
                        strcpy(descriptor, define);  
                    else  
                    {  
                        strcat(descriptor, "/");  
                        strcat(descriptor, define);  
                    }  
                    break;  
                }  
            }  
        }  
        fclose(in_file);  
        while (ptr = strchr(descriptor, '_')) *ptr = ' ';  
    }  
    static void report_simple_object_info(char *what, tag_t object)  
    {  
        int  
            status,  
            subtype,  
            type;  
        tag_t  
            owner;  
        char  
            name[UF_OBJ_NAME_LEN + 1],  
            owning_part[UF_CFI_MAX_FILE_NAME_SIZE],  
            msg[133];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "\n%s = %d", what, object);  
        UF_CALL(UF_UI_write_listing_window(msg));  
        if (object == NULL_TAG)  
        {  
            UF_CALL(UF_UI_write_listing_window("\n"));  
            return;  
        }  
        status = UF_OBJ_ask_status(object);  
        if (status == UF_OBJ_DELETED)  
        {  
            sprintf(msg, ", status: %d\n", status);  
            UF_CALL(UF_UI_write_listing_window(msg));  
            return;  
        }  
        if (!UF_OBJ_ask_name(object, name))  
        {  
            UF_UI_write_listing_window(", named: ");  
            UF_UI_write_listing_window(name);  
        }  
        UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
        ask_object_type_descriptor(type, subtype, msg);  
        UF_UI_write_listing_window(", type: ");  
        UF_UI_write_listing_window(msg);  
        if (UF_OBJ_is_object_a_promotion(object))  
            UF_UI_write_listing_window(", promotion");  
        if (UF_ASSEM_is_occurrence(object))  
            UF_UI_write_listing_window(", occurrence");  
        if (!UF_CALL(UF_OBJ_ask_owning_part(object, &owner)))  
        {  
            ask_part_shortname(owner, owning_part);  
            UF_UI_write_listing_window(", in: ");  
            UF_UI_write_listing_window(owning_part);  
        }  
        UF_UI_write_listing_window("\n");  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_objects,  
            new_layer;  
        tag_t  
            *objects;  
        UF_CALL(UF_LAYER_ask_work_layer(&new_layer));  
        while (((n_objects = select_objects("Move to Layer", &objects)) > 0) &&  
            prompt_for_a_layer("Destination", "Layer", &new_layer))  
        {  
            for (ii = 0; ii < n_objects; ii++)  
            {  
                RX(objects[ii]);  
                UF_CALL(UF_OBJ_set_layer(objects[ii], new_layer));  
            }  
            UF_free(objects);  
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

> 这段代码是一个NX二次开发程序，其主要功能是选择对象并将它们移动到指定的图层。以下是代码的主要功能：
>
> 1. 定义了错误报告函数report_error，用于输出错误代码、消息和调用函数。
> 2. 定义了选择对象的函数select_objects，用于弹出选择对话框，让用户选择要移动的对象。
> 3. 定义了提示用户输入图层号的函数prompt_for_a_layer。
> 4. 定义了获取对象类型描述的函数ask_object_type_descriptor。
> 5. 定义了报告对象信息的函数report_simple_object_info。
> 6. do_it函数实现了主要功能：提示用户选择对象，提示输入目标图层号，然后将选中的对象移动到指定图层。
> 7. ufusr函数是程序的入口函数，初始化NX环境，调用do_it函数执行功能，然后终止NX环境。
> 8. ufusr_ask_unload函数用于设置程序卸载方式。
>
> 总的来说，这段代码通过交互式选择和提示，实现了将对象移动到指定图层的目的。
>
