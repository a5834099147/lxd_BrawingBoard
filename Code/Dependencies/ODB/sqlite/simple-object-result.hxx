// file      : odb/sqlite/simple-object-result.hxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : GNU GPL v2; see accompanying LICENSE file

#ifndef ODB_SQLITE_SIMPLE_OBJECT_RESULT_HXX
#define ODB_SQLITE_SIMPLE_OBJECT_RESULT_HXX

#include <odb/pre.hxx>

#include <cstddef> // std::size_t

#include <odb/schema-version.hxx>
#include <odb/simple-object-result.hxx>

#include <odb/details/shared-ptr.hxx>

#include <odb/sqlite/version.hxx>
#include <odb/sqlite/forward.hxx> // query_base, query_params
#include <odb/sqlite/statement.hxx>
#include <odb/sqlite/traits-calls.hxx>

namespace odb
{
  namespace sqlite
  {
    template <typename T>
    class object_result_impl: public odb::object_result_impl<T>
    {
    public:
      typedef odb::object_result_impl<T> base_type;

      typedef typename base_type::id_type id_type;
      typedef typename base_type::object_type object_type;
      typedef typename base_type::pointer_type pointer_type;

      typedef object_traits_impl<object_type, id_sqlite> object_traits;
      typedef typename base_type::pointer_traits pointer_traits;

      typedef typename object_traits::statements_type statements_type;

      virtual
      ~object_result_impl ();

      object_result_impl (const query_base&,
                          const details::shared_ptr<select_statement>&,
                          statements_type&,
                          const schema_version_migration*);

      virtual void
      load (object_type&, bool fetch);

      virtual id_type
      load_id ();

      virtual void
      next ();

      virtual void
      cache ();

      virtual std::size_t
      size ();

      virtual void
      invalidate ();

      using base_type::current;

    private:
      void
      load_image ();

    private:
      // We need to hold on to the query parameters because SQLite uses
      // the parameter buffers to find each next row.
      //
      details::shared_ptr<query_params> params_;
      details::shared_ptr<select_statement> statement_;
      statements_type& statements_;
      object_traits_calls<object_type> tc_;
    };
  }
}

#include <odb/sqlite/simple-object-result.txx>

#include <odb/post.hxx>

#endif // ODB_SQLITE_SIMPLE_OBJECT_RESULT_HXX
