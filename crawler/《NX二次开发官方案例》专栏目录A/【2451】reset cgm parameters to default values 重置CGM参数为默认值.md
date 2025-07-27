### 【2451】reset cgm parameters to default values 重置CGM参数为默认值

#### 代码

```cpp
    static void reset_cgm_parameters_to_default_values(tag_t part)  
    {  
        int     
            type,   
            subtype;  
        tag_t  
            object = NULL_TAG;  
        while (! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_parameter_type, &object))   
            &&  object != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype( object, &type, &subtype));  
            if (subtype == UF_parm_diagram_subtype)  
            {  
                UF_CALL(UF_OBJ_delete_object(object));  
                break;  
            }  
        }                        
    }

```

#### 代码解析

> 这是段NX二次开发代码，其主要功能是重置指定部件(part)的CGM参数默认值。
>
> 代码步骤如下：
>
> 1. 定义了部件(part)的tag_t类型的参数。
> 2. 使用UF_OBJ_cycle_objs_in_part()函数循环遍历部件中的对象，判断是否为参数对象。
> 3. 如果是参数对象，则通过UF_OBJ_ask_type_and_subtype()函数获取参数对象的类型和子类型。
> 4. 如果参数对象的子类型为UF_parm_diagram_subtype，则表示为CGM参数，此时会调用UF_OBJ_delete_object()函数删除该参数对象。
> 5. 删除首个匹配的CGM参数后，循环结束。
> 6. 通过此过程，该部件的CGM参数将被重置为默认值。
>
> 该代码的主要功能是遍历部件中的参数对象，判断并删除CGM参数对象，从而实现重置CGM参数默认值的目的。
>
