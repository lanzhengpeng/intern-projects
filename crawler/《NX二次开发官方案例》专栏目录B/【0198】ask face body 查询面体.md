### 【0198】ask face body 查询面体

#### 代码

```cpp
    static tag_t ask_face_body(tag_t face)  
    {  
        tag_t  
            body,  
            occ;  
        UF_CALL(UF_MODL_ask_face_body(face, &body));  
        if (UF_ASSEM_is_occurrence(face) && !UF_ASSEM_is_occurrence(body))  
        {  
            occ = UF_ASSEM_ask_part_occurrence(face);  
            body = UF_ASSEM_find_occurrence(occ, body);  
        }  
        return body;  
    }

```

#### 代码解析

> 这段NX Open C++代码用于查询一个面的体。
>
> 主要思路是：
>
> 1. 首先调用UF_MODL_ask_face_body函数，传入面的tag，查询该面所属的体。
> 2. 然后判断面是否是装配中的面，如果是，还需要找到该体在当前装配中的实例。
> 3. 最终返回面的体的tag。
>
> 代码逻辑如下：
>
> 1. 调用UF_MODL_ask_face_body函数，查询面的体。
> 2. 判断面是否是装配中的面，如果是，则获取面的装配实例tag。
> 3. 根据面的装配实例tag，找到该体在当前装配中的实例tag。
> 4. 最终返回该体的tag。
>
> 该函数可以用于在装配环境中，获取一个面所属的体，并返回其在当前装配中的实例。
>
