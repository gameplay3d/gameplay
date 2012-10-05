/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __DAE_SIDRESOLVER_H__
#define __DAE_SIDRESOLVER_H__

#include <string>
#include <map>
#include <dae/daeTypes.h>
#include <dae/daeElement.h>


// This is an alternative to the daeSIDResolver class. It's recommended you use
// this class instead. Typical usage: get the element a sid ref points to. For
// example, if you want to find the element with sid 'sampler' using a
// daeElement pointer named 'effect', that would look like this:
//   daeElement* elt = daeSidRef("sampler", effect).resolve().elt
struct DLLSPEC daeSidRef {
	// A helper class for returning all the data retrieved when a sid is resolved.
	struct DLLSPEC resolveData {
		resolveData();
		resolveData(daeElement* elt, daeDoubleArray* array, daeDouble* scalar);

		daeElement* elt;
		daeDoubleArray* array;
		daeDouble* scalar;
	};
	
	daeSidRef();
	daeSidRef(const std::string& sidRef, daeElement* referenceElt, const std::string& profile = "");
	bool operator<(const daeSidRef& other) const;

	resolveData resolve();
		
	std::string sidRef;
	daeElement* refElt;
	std::string profile;
};


/**
 * The daeSIDResolver class is designed to resolve sid references within a COLLADA document.
 * The rules for sid resolution are set forth by the Addressing Syntax section in Chapter 3 of the
 * COLLADA specification which can be found at https://www.khronos.org/collada .
 * This resolver always attempts to resolve to the daeElement which is referenced. If the element contains 
 * a daeDoubleArray (domFloatArray) value, the resolver will set the pointer to that array. The
 * resolver will also do this if the sid target points to a <source> element which has a <float_array> as
 * a child. If the sid target specifies a value, i.e. blah.X or blah(6), the resolver will attempt to
 * get a pointer to that specific value. The resolver only attempts to resolve to that level for values which
 * are defined in the COMMON profile glossary of the COLLADA specification, or values reference with the (#)
 * syntax. You can check the return value from getState() to see which level of resolution is possible.
 */
class DLLSPEC daeSIDResolver
{
public:
	/**
	 * An enum describing the status of the SID resolution process.
	 */
	enum ResolveState{
		/** No target specified */
		target_empty,
		/** target specified but not resolved */
		target_loaded,
		/** Resolution failed because target was not found */
		sid_failed_not_found,
		/** Resolution successful to the Element level */
		sid_success_element,
		/** Resolution successful to the Double Array level */
		sid_success_array,
		/** Resolution successful to the Double level */
		sid_success_double
	};

	/**
	 * Constructor.
	 * @param container The element which contains the target that you want to resolve.
	 * @param target The target string which needs to be resolved.
	 * @param platform The platform name of the technique to use. A NULL value indicates the common platform.
	 */
	daeSIDResolver( daeElement *container, daeString target, daeString platform = NULL );

	/**
	 * Gets the target string.
	 * @return Returns the target string of this SID resolver.
	 */
	daeString getTarget() const;
	/**
	 * Sets the target string.
	 * @param t The new target string for this resolver.
	 */
	void setTarget( daeString t );

	/**
	 * Gets the name of the profile to use when resolving.
	 * @return Returns the name of the profile or NULL for the common profile.
	 */
	daeString getProfile()	const;
	/**
	 * Sets the profile to use when resolving.
	 * @param p The profile name of the technique to use. A NULL value indicates the common profile.
	 */
	void setProfile( daeString p );

	/**
	 * Gets a pointer to the @c daeElement that contains the target to resolve.
	 * @return Returns the pointer to the containing daeElmement.
	 */
	daeElement* getContainer() const;
	/**
	 * Sets the pointer to the @c daeElement that contains the target to resolve.
	 * @param element Pointer to the containing @c daeElmement.
	 */
	void setContainer(daeElement* element);

	/**
	 * Gets the element that this SID resolves to.
	 * @return Returns the element that the URI resolves to.
	 */
	daeElement* getElement();

	/**
	 * Gets the value array of the element that the SID resolves to.
	 * @return Returns a pointer to the value array that the SID resolves to
	 * @note The daeSIDResolver can only resolve to this level for daeDoubleArray values.
	 */
	daeDoubleArray *getDoubleArray();

	/**
	 * Gets a pointer to the particle this target resolved to.
	 * @return Returns a pointer to a double value which is the fully resolved target.
	 * @note The daeSIDResolver can only resolve to this level for domDouble values and only if the
	 * final symbolic name is from the COMMON profile or a cardinal value is specified.
	 * @note The daeSIDResolver assumes the value is a 4x4 matrix if there are 2 cardinal values specified.
	 */
	daeDouble *getDouble();

	// This method is deprecated. Don't use it.
	ResolveState getState() const;

private:
	// This data is provided by the user
	std::string	target;
	std::string	profile;	
	daeElement* container;
};


// A class to make sid ref lookups faster. Meant for DOM internal use only.
class DLLSPEC daeSidRefCache {
public:
	daeSidRefCache();
	~daeSidRefCache() { };
	
	daeSidRef::resolveData lookup(const daeSidRef& sidRef);
	void add(const daeSidRef& sidRef, const daeSidRef::resolveData& data);
	void clear();

	// For debugging/testing
	bool empty();
	int misses();
	int hits();

private:
	std::map<daeSidRef, daeSidRef::resolveData> * lookupTable;
	int hitCount;
	int missCount;
};

#endif

