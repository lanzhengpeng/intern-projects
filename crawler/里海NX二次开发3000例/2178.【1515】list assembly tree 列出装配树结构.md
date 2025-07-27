### 【1515】list assembly tree 列出装配树结构

#### 代码

```cpp
    static logical is_object_a_component_member(tag_t component, tag_t object)  
    {  
        tag_t  
            obj = NULL_TAG;  
        while (!UF_CALL(UF_ASSEM_cycle_objs_in_comp(component, &obj)) &&  
            (obj != NULL_TAG)) if (obj == object) return TRUE;  
        return FALSE;  
    }  
    static logical is_object_blanked(tag_t object)  
    {  
        UF_OBJ_disp_props_t  
            disp_props;  
        UF_CALL(UF_OBJ_ask_display_properties(object, &disp_props));  
        return (logical) disp_props.blank_status;  
    }  
    static void list_assembly_tree(tag_t component, int indent, char *fmt)  
    {  
        int  
            ii,  
            n_children;  
        tag_t  
            *child_components;  
        char  
            *check_color[2] = { "red", "grey" },  
            comp_name[30+1],  
            msg[133],  
            push[133];  
        n_children = UF_ASSEM_ask_part_occ_children(component, &child_components);  
        if (n_children > 0)  
        {  
            fmt[indent * 3] = '|';  
            for (ii = 0; ii < n_children; ii++)  
            {  
                ask_component_shortname(child_components[ii], comp_name);  
                strncpy(push, fmt, (indent * 3));  
                push[indent * 3] = '\0';  
                sprintf(msg, "%s|__%s (%s check)\n", push, comp_name,  
                    check_color[!is_object_a_component_member(component,  
                        child_components[ii]) ||  
                        is_object_blanked(child_components[ii])]);  
                ECHO(msg);  
                if (ii == (n_children - 1)) fmt[indent * 3] = ' ';  
                list_assembly_tree(child_components[ii], indent + 1, fmt);  
            }  
            UF_free(child_components);  
        }  
    }

```

#### 代码解析

> 这段代码是一个用于遍历NX装配树并输出树结构的示例，具体功能如下：
>
> 1. 组件成员判断函数：is_object_a_component_member用于判断一个对象是否为组件的成员。
> 2. 对象是否被隐藏的判断函数：is_object_blanked用于判断一个对象是否被隐藏。
> 3. 递归遍历装配树函数：list_assembly_tree是一个递归函数，用于遍历NX装配树并输出树结构。
>
> 主要逻辑包括：
>
> 该代码通过递归遍历装配树，并以树形结构输出每个组件的信息，同时通过不同的颜色标识组件的状态，有助于直观地查看装配结构。
>
