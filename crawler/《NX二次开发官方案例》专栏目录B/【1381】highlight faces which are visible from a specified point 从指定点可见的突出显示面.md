### 【1381】highlight faces which are visible from a specified point 从指定点可见的突出显示面

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_linked_exterior.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_so.h>  
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
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype) return body;  
        }  
        return NULL_TAG;  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static int ask_all_bodies(tag_t part, tag_t **solids)  
    {  
        tag_t  
            body = NULL_TAG;  
        uf_list_p_t  
            body_list;  
        UF_CALL(UF_MODL_create_list(&body_list));  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(body_list, body));  
        return (make_an_array(&body_list, solids));  
    }  
    static logical specify_point(char *prompt, double loc[3])  
    {  
        int  
            ia2[2] = { 0,0 },  
            ip3 = 0,  
            irc;  
        irc = uc1616(prompt, ia2, ip3, loc);  
        if (irc == 5) return TRUE;  
        else return FALSE;  
    }  
    static void set_highlight_object_array(int n, tag_t *objects, int flag)  
    {  
        int  
            ii,  
            subtype,  
            type;  
        for (ii = 0; ii < n; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(objects[ii], &type, &subtype));  
            if (type == UF_occ_instance_type)  
                UF_CALL(UF_DISP_set_highlight(UF_ASSEM_ask_part_occ_of_inst(  
                    NULL_TAG, objects[ii]), flag));  
            else  
                UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
        }  
    }  
    static void display_temporary_text(char *text, double loc[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static void number_objects(tag_t *objects, int n_objects)  
    {  
        logical  
            is_displayable;  
        int  
            ii;  
        double  
            loc[3];  
        char  
            msg[133];  
        UF_DISP_refresh();  
        for (ii = 0; ii < n_objects; ii++)  
        {  
            UF_CALL(UF_OBJ_is_displayable(objects[ii], &is_displayable));  
            if (!is_displayable) continue;  
            sprintf(msg, "%d", ii+1);  
            if (UF_OBJ_ask_name_origin(objects[ii], loc))  
            {  
                UF_CALL(UF_OBJ_set_name(objects[ii], "X"));  
                UF_CALL(UF_OBJ_ask_name_origin(objects[ii], loc));  
                UF_CALL(UF_OBJ_delete_name(objects[ii]));  
            }  
            display_temporary_text(msg, loc);  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            *body_index = NULL,  
            ii,  
            n_bodies,  
            n_faces;  
        tag_t  
            *bodies,  
            *comps,  
            *faces,  
            *xforms;  
        double  
            origin[3],  
            chordal_tol;  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_MODL_ask_distance_tolerance(&chordal_tol);  
        n_bodies = ask_all_bodies(UF_PART_ask_display_part(), &bodies);  
        if (n_bodies == 0)  
        {  
            ECHO("%s\n", "This part has no solids");  
            return;  
        }  
        if (UF_CALL(allocate_memory(n_bodies * sizeof(tag_t), &xforms)))  
            return;  
        if (UF_CALL(allocate_memory(n_bodies * sizeof(tag_t), &comps)))  
            return;  
        for (ii = 0; ii < n_bodies; ii++)  
        {  
            if (UF_ASSEM_is_occurrence(bodies[ii]))  
            {  
                comps[ii] = UF_ASSEM_ask_part_occurrence(bodies[ii]);  
                UF_CALL(UF_SO_create_xform_assy_ctxt(comps[ii],  
                    comps[ii], NULL_TAG, &xforms[ii]));  
                bodies[ii] = UF_ASSEM_ask_prototype_of_occ(bodies[ii]);  
            }  
            else  
            {  
                xforms[ii] = NULL_TAG;  
                comps[ii] = NULL_TAG;  
            }  
        }  
        if (specify_point("Where are you looking from?", origin))  
        {  
            UF_CALL(UF_MODL_identify_exterior_using_rays(n_bodies, bodies,  
                xforms, origin, chordal_tol, UF_LINKED_RAY_FROM_POINT,  
                &n_faces, &faces, &body_index));  
            if (n_faces > 0)  
            {  
                for (ii = 0; ii < n_faces; ii++)  
                {  
                    if (comps[body_index[ii]] != NULL_TAG)  
                        faces[ii] = UF_ASSEM_find_occurrence(comps[body_index[ii]],   
                            faces[ii]);  
                }  
                set_highlight_object_array(n_faces, faces, TRUE);  
                number_objects(faces, n_faces);  
                sprintf(msg, "%d faces can be seen from there", n_faces);  
                uc1601(msg, TRUE);  
                set_highlight_object_array(n_faces, faces, FALSE);  
                UF_free(faces);  
                UF_free(body_index);  
            }  
        }  
        UF_free(bodies);  
        UF_free(xforms);  
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

> 这段代码是用于NX的二次开发代码，主要实现了以下功能：
>
> 1. 错误处理和日志输出：代码定义了错误处理函数ECHO和report_error，用于输出错误信息和日志。
> 2. 遍历和查询实体：ask_next_body函数用于遍历零件中的实体，并返回下一个实体。ask_all_bodies函数用于查询零件中的所有实体，并返回实体数组。
> 3. 内存分配和列表处理：allocate_memory函数用于分配内存，make_an_array函数用于将列表转换为数组。
> 4. 用户输入处理：specify_point函数用于提示用户输入一个点。
> 5. 高亮显示：set_highlight_object_array函数用于设置对象的高亮显示。
> 6. 文本显示：display_temporary_text函数用于在指定位置显示临时文本。
> 7. 对象编号：number_objects函数用于在指定位置显示对象的编号。
> 8. 主逻辑：do_it函数实现了主要逻辑，包括查询当前显示零件的所有实体，根据用户输入的点，识别出可见的实体面，并高亮显示和编号。
> 9. NX接口：代码中使用了NX提供的API接口，如UF_MODL、UF_OBJ、UF_DISP等。
> 10. NX启动和退出：ufusr函数用于NX启动和退出时调用。
>
> 总体来说，这段代码实现了一个简单的NX二次开发程序，用于遍历查询零件中的实体，并根据用户输入的点识别出可见的实体面，并进行高亮显示和编号。
>
