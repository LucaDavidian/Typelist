#ifndef HIERARCHY_GEN_H
#define HIERARCHY_GEN_H

#include "typelist.hpp"

// scatter hierachy
template <typename T, template <typename> typename Unit>
class GenScatterHierarchy;

template <typename Head, typename... Tail, template <typename> class Unit>
class GenScatterHierarchy<Typelist<Head, Tail...>, Unit> : public Unit<Head>, public GenScatterHierarchy<Typelist<Tail...>, Unit>
{
}; 

template <template <typename> typename Unit>
class GenScatterHierarchy<Typelist<>, Unit>
{
};

// linear hierarchy (Unit must inherit from Base)
class EmptyType {};

template <typename T, template <typename Type, typename Base> class Unit, typename Root = EmptyType>
class GenLinearHierarchy;

template <typename Head, typename... Tail, template <typename, typename> class Unit, typename Root>
class GenLinearHierarchy<Typelist<Head, Tail...>, Unit, Root> : public Unit<Head, GenLinearHierarchy<Typelist<Tail...>, Unit, Root>>
{ 
};

template <template <typename Type, typename Base> class Unit, typename Root>
class GenLinearHierarchy<Typelist<>, Unit, Root> : public Root
{
};

#endif  // HIERARCHY_GEN_H