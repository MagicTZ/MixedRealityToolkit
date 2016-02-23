//////////////////////////////////////////////////////////////////////////
// SyncPrimitives.h
//
// Copyright (C) 2015 Microsoft Corp.  All Rights Reserved
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "Syncable.h"

XTOOLS_NAMESPACE_BEGIN

template<typename T> struct ElementEnum { static const ElementType Type = ElementType::UnknownType; };
template<> struct ElementEnum<IntElement> { static const ElementType Type = ElementType::Int32Type; };
template<> struct ElementEnum<LongElement> { static const ElementType Type = ElementType::Int64Type; };
template<> struct ElementEnum<FloatElement> { static const ElementType Type = ElementType::FloatType; };
template<> struct ElementEnum<DoubleElement> { static const ElementType Type = ElementType::DoubleType; };
template<> struct ElementEnum<StringElement> { static const ElementType Type = ElementType::StringType; };


/// Templated class for wrapping the sync functionality of primitive types (ie: non-object types)
template<typename primitiveType, typename elementType>
class SyncPrimitiveBase : public Syncable
{
public:
	typedef SyncPrimitiveBase<primitiveType, elementType> this_type;

	SyncPrimitiveBase(primitiveType initialValue) : m_value(initialValue) {}

	XGuid GetGUID() const XTOVERRIDE
	{
		if (m_element) { return m_element->GetGUID(); }
		else { return kInvalidXGuid; }
	}

	ElementType GetType() const XTOVERRIDE { return ElementEnum<elementType>::Type; }

	const ref_ptr<elementType>& GetElement() { return m_element; }


	primitiveType Get() const { return m_value; }

	void Set(primitiveType f)
	{
		if (f != m_value)
		{
			m_value = f;
			if (m_element) m_element->SetValue(m_value);
		}
	}

	this_type& operator=(primitiveType rhs)
	{
		Set(rhs);
		return *this;
	}

	operator const primitiveType() const { return Get(); }


protected:
	virtual void BindLocal(const ObjectElementPtr& parent, const std::string& name, const UserPtr&) XTOVERRIDE
	{
		m_element = CreateElement(parent, name, m_value);
		if (!XTVERIFY(m_element != nullptr))
		{
			LogError("Failed to create sync element for object %s", name.c_str());
		}
	}


	virtual void BindRemote(const ElementPtr& element) XTOVERRIDE
	{
		m_element = elementType::Cast(element);
		if (XTVERIFY(m_element != nullptr))
		{
			m_value = m_element->GetValue();
		}
		else
		{
			LogError("Failed to bind sync float element");
		}
	}


	virtual void SetValue(primitiveType newValue) 
	{
		m_value = newValue;
	}

private:
	static FloatElementPtr	CreateElement(const ObjectElementPtr& parent, const std::string& name, float value) { return  parent->CreateFloatElement(new XString(name), value); }
	static DoubleElementPtr	CreateElement(const ObjectElementPtr& parent, const std::string& name, double value) { return  parent->CreateDoubleElement(new XString(name), value); }
	static IntElementPtr	CreateElement(const ObjectElementPtr& parent, const std::string& name, int32 value) { return  parent->CreateIntElement(new XString(name), value); }
	static LongElementPtr	CreateElement(const ObjectElementPtr& parent, const std::string& name, int64 value) { return  parent->CreateLongElement(new XString(name), value); }
	static StringElementPtr	CreateElement(const ObjectElementPtr& parent, const std::string& name, const XStringPtr& value) { return  parent->CreateStringElement(new XString(name), value); }

	ref_ptr<elementType> m_element;
	primitiveType m_value;
};


typedef SyncPrimitiveBase<float, FloatElement> SyncFloat;
typedef SyncPrimitiveBase<double, DoubleElement> SyncDouble;
typedef SyncPrimitiveBase<int32, IntElement> SyncInt;
typedef SyncPrimitiveBase<int64, LongElement> SyncLong;
typedef SyncPrimitiveBase<XStringPtr, StringElement> SyncString;

XTOOLS_NAMESPACE_END
