/*
 * Copyright (C) 2017 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#ifndef IGNITION_PHYSICS_ENTITY_HH_
#define IGNITION_PHYSICS_ENTITY_HH_

#include <memory>
#include <limits>

#include <ignition/physics/Export.hh>
#include <ignition/physics/Feature.hh>

namespace ignition
{
  namespace physics
  {
    /// \brief This constant-value should be used to indicate that an Entity ID
    /// is invalid (i.e. does not refer to a real entity).
    const std::size_t INVALID_ENTITY_ID =
        std::numeric_limits<std::size_t>::max();

    /// \brief This is the base class of all "proxy objects". The "proxy
    /// objects" are essentially interfaces into the actual objects which exist
    /// inside of the various physics engine implementations. The proxy objects
    /// contain the minimal amount of data (e.g. a unique identifier,
    /// a reference-counter for the implementation object, and a reference to
    /// the implementation interface that it needs) necessary to interface with
    /// the object inside of the implementation that it refers to.
    ///
    /// Examples of proxy objects are the Link class, Joint class, and Model
    /// class.
    template <typename FeatureType, typename PimplT>
    class IGNITION_PHYSICS_VISIBLE Entity
    {
      /// \brief Get the unique entity ID of this Link.
      public: std::size_t EntityID() const;

      /// \brief Get a reference-counting std::shared_ptr to the object inside
      /// the implementation that this object provides an abstraction for.
      public: const std::shared_ptr<const void> &EntityReference() const;

      /// \brief Constructor for the Entity.
      ///
      /// Notes for developers:
      /// - We provide a default constructor for this class so that the feature
      /// entity classes (which get virtually inherited) do not each need to
      /// call on the constructor of Entity. That would make it difficult
      /// to implement and maintain all the constructors of the different object
      /// feature classes.
      /// - Since all the features are virtually inherited, only the "final"
      /// inheriting class constructor needs to actually call this constructor.
      /// - The default argument for the ID is the highest possible integer.
      /// This should help to make it clear if the construction procedure is not
      /// working as intended. If _pimpl is a nullptr, that would also
      /// indicate that the construction procedure is not working as intended.
      protected: Entity(
        const std::shared_ptr<PimplT> &_pimpl = nullptr,
        const std::size_t _id = INVALID_ENTITY_ID,
        const std::shared_ptr<const void> &_ref = nullptr);

      /// \brief This is a pointer to the physics engine implementation, and it
      /// can be used by the object features to find the interfaces that they
      /// need in order to function.
      protected: std::shared_ptr<PimplT> pimpl;

      /// \brief This integer ID uniquely identifies the object that this entity
      /// is referring to. No two entities may use the same ID unless they are
      /// referring to the same instance of a physics engine object.
      ///
      /// The ID is not allowed to change at any point in the lifetime of the
      /// engine object.
      ///
      /// Note that the ID of 0 is reserved for the "engine" object.
      private: const std::size_t id;

      /// \brief This is an optional reference-counting field for the proxy
      /// objects. Not all engines are required to support this field for all
      /// types, so this may be left as a nullptr.
      ///
      /// This reference is not allowed to change at any point in the lifetime
      /// of the engine object.
      private: std::shared_ptr<const void> ref;

      /// \brief Virtual destructor
      public: virtual ~Entity() = default;
    };
  }
}

#include <ignition/physics/detail/Entity.hh>

#endif