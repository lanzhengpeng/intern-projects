### 【2719】set all face grid counts 设置所有面的网格数量

#### 代码

```cpp
    /*HEAD SET_ALL_FACE_GRID_COUNTS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据上下文，这句话的意思是：请注意，UF_print_syslog 是 V18 中的新增功能。只提供翻译，不要添加其他无关的内容。 */  
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
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        do  
        {  
            UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body));  
            if (body)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
                if (!subtype) return body;  
            }  
        } while (body);  
        return NULL_TAG;  
    }  
    static int ask_body_faces(tag_t body, tag_p_t *body_faces)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *faces;  
        uf_list_p_t  
            face_list;  
        UF_CALL(UF_MODL_ask_body_faces(body, &face_list));  
        UF_CALL(UF_MODL_ask_list_count(face_list, &n));  
        *body_faces = (faces = (tag_t *)malloc(n * sizeof(tag_t)));  
        for (ii = 0; ii < n; ii++)  
        {  
            UF_CALL(UF_MODL_ask_list_item(face_list, ii, &faces[ii]));  
            if (UF_ASSEM_is_occurrence(body) && !UF_ASSEM_is_occurrence(faces[ii]))  
                faces[ii] = UF_ASSEM_find_occurrence(  
                    UF_ASSEM_ask_part_occurrence(body), faces[ii]);  
        }  
        UF_CALL(UF_MODL_delete_list(&face_list));  
        return n;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_faces,  
            uvcounts[2] = { 0, 0 };  
        tag_t  
            body = NULL_TAG,  
            *faces,  
            part = UF_PART_ask_display_part();  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
        {  
            n_faces = ask_body_faces(body, &faces);  
            for (ii = 0; ii < n_faces; ii++)  
                FTN(uf5401)(&faces[ii], uvcounts);  
            free(faces);  
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

> 这段NX二次开发代码的功能是遍历当前显示部件中的所有实体，并对每个实体进行以下操作：
>
> 1. 遍历实体中的所有面。
> 2. 对每个面调用函数FTN(uf5401)来设置其UV网格数量。
> 3. 释放分配的面数组内存。
>
> 具体来说，代码包含以下关键部分：
>
> 通过以上步骤，代码实现了对当前显示部件中所有实体的面进行UV网格数量设置的功能。
>
