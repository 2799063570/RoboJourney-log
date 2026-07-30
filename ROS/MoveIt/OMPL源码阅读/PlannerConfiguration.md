# PlannerConfiguration：规划器参数模型

## 结论

MoveIt 将规划组、配置名称和键值参数组织为 `PlannerConfigurationSettings`，再由 `PlannerConfigurationMap` 按名称查找。YAML 中的参数最终需要与规划器插件读取的 key 一致；配置存在不等于该规划器一定可被加载。

## 算法参数的存储类型

```cpp
struct PlannerConfigurationSettings
{
  /** \brief The group (as defined in the SRDF) this configuration is meant for */
  std::string group;

  /* \brief Name of the configuration.

     For a group's default configuration, this should be the same as the group name.
     Otherwise, the form "group_name[config_name]" is expected for the name. */
  std::string name;

  /** \brief Key-value pairs of settings that get passed to the planning algorithm */
  std::map<std::string, std::string> config;
};
/** \brief Map from PlannerConfigurationSettings.name to PlannerConfigurationSettings */
typedef std::map<std::string, PlannerConfigurationSettings> PlannerConfigurationMap;
```


## 固定解析规则（MoveIt 官方源码逻辑）

1. **`group`** = YAML 顶层规划组名 → `arm`
2. **`name`** = 固定格式：`组名[规划器名]` → `arm[RRTConnect]`、`arm[PRMstar]`
3. **`config`**
    
    - 合并 **规划组参数** + **规划器专属参数**
    - 所有值**转字符串**，塞进 `std::map<std::string, std::string>`
    
4. **`PlannerConfigurationMap` 的 key** = `name`（和结构体里的 name 完全一致）

## 举个例子

```yaml
arm: 
  default_planner_config: RRTConnect
  planner_configs: 
    - RRTConnect 
    - PRMstar 
  projection_evaluator: joints(shoulder_joint,upperArm_joint)
  longest_valid_segment_fraction: 0.005

planner_configs: 
  RRTConnect: 
    type: geometric::RRTConnect 
    range: 0.05 
  PRMstar: 
    type: geometric::PRMstar 
    goal_bias: 0.05
```

则可以得到

```cpp
PlannerConfigurationMap.insert( {arm[RRTConnect], {group:arm, name:arm[RRTConnect], {type:geometric::RRTConnect , range: 0.05} } });
PlannerConfigurationMap.insert( {arm[PRMstar], {group:arm, name:arm[PRMstar], {type:geometric::PRMstar , range: 0.05} } });
```

详细见[[PlanningInterface]]
