#pragma once

#include <Windows.h>
#include <memory>
#include <string>
#include <vector>

template<typename t> using ptr = std::unique_ptr<t>;

namespace settings {
	
	class c_setting {
	public:

		std::string m_id;
		
		virtual void load_value( std::string ) = 0;
		virtual std::string export_value( void ) = 0;

		c_setting( void ) { }
		c_setting( std::string );
	};

	class c_boolean : public c_setting {
	public:

		bool value{};

		void load_value( std::string );
		std::string export_value( void );

		c_boolean( std::string, bool );


	};

	class c_float : public c_setting {
	public:

		float value{};

		void load_value( std::string );
		std::string export_value( void );

		c_float( std::string, float );

	};

	class c_string : public c_setting {
	public:

		std::string value{};

		void load_value( std::string );
		std::string export_value( void );

		c_string( std::string, std::string );

	};


	class c_int : public c_setting {
	public:

		int value{};

		void load_value( std::string );
		std::string export_value( void );

		c_int( std::string, int );


	};

	class c_manager {
	public:

		static c_manager* get( void );

		c_manager( void );

		void load_values( std::vector<std::string> );
		void load_config( std::string );
		void export_config( std::string );

		template<typename t> t* find_setting( std::string id ) {

			for ( auto v : this->m_settings )
			{

				if ( v->m_id == id ) return reinterpret_cast< t* >( v );

			}

			return nullptr;

		}

		std::vector<c_setting*> m_settings;

		c_boolean* boolean( std::string, bool = false );
		c_float* floating_point( std::string, float = 0.f );
		c_string* string( std::string, std::string = "" );
		c_int* integer( std::string, int = 0 );
	};

}

typedef settings::c_manager settings_manager;