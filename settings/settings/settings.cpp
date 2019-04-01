#include "settings.h"
#include <fstream>

float shift_decimals( float x, int places ) {

	return x * std::powf( 10, places );

}

int get_int_from_char( char character ) {

	return static_cast< int >( character - '0' );

}


settings::c_manager* settings::c_manager::get( void )
{

	static ptr<c_manager> instance = std::make_unique<c_manager>( );

	return instance.get( );

}

settings::c_setting::c_setting( std::string id ) {

	this->m_id = id;

}

settings::c_boolean::c_boolean( std::string id, bool initial ) : c_setting(id) {

	this->value = initial;

}

settings::c_manager::c_manager( void ) {

}

settings::c_boolean* settings::c_manager::boolean( std::string id, bool init ) {

	auto boolean = new c_boolean( id, init );

	this->m_settings.push_back(boolean );

	return boolean;

}

settings::c_float* settings::c_manager::floating_point( std::string id, float value ) {

	auto floating = new c_float( id, value );

	this->m_settings.push_back( floating );

	return floating;

}

settings::c_string* settings::c_manager::string( std::string id, std::string value ) {

	auto s = new c_string( id, value );

	this->m_settings.push_back( s );

	return s;

}

settings::c_int* settings::c_manager::integer( std::string id, int value ) {

	auto var = new c_int( id, value );

	this->m_settings.push_back( var );

	return var;

}

void settings::c_manager::load_config( std::string name ) {

	auto path = "C:\\" + name + ".integral";

	std::ifstream ofs;
	ofs.open( path );

	std::vector<std::string> lines{};

	auto get_lines = [ &ofs ]( std::vector<std::string>* lines ) {

		std::string line;

		while ( ofs.is_open( ) && std::getline( ofs, line ) ) {

			lines->push_back( line );

		}
	};

	get_lines( &lines );

	this->load_values( lines );

}

void settings::c_manager::export_config( std::string config ) {

	auto path = "C:\\" + config + ".integral";

	std::ofstream ofs;
	ofs.open( path, std::ofstream::out | std::ofstream::trunc );

	auto get_export = [ & ]( void ) {

		std::string exported = "";

		for ( int i = 0; i < this->m_settings.size( ); i++ ) {

			auto setting = this->m_settings[ i ];

			exported += "[" + setting->m_id + "] " + setting->export_value( );

			if ( i != ( this->m_settings.size( ) - 1 ) ) exported += "\n";

		}

		return exported;

	};

	auto exported = get_export( );

	DWORD written;

	ofs.write( exported.c_str( ), exported.size( ) );

	ofs.close( );


}

void settings::c_manager::load_values( std::vector<std::string> lines ) {

	auto get_command = [ ]( std::string line ) {

		int id_start_index = line.find_first_of( "[" ) + 1, id_end_index = line.find_first_of( "]" ) - 1;

		auto start_command_index = line.find_first_of( " " ) + 1;

		return std::pair<std::string, std::string>{ line.substr( id_start_index, id_end_index ), line.substr( start_command_index, line.size( ) ) };

	};

	for ( auto v : lines ) {

		auto data = get_command( v );
		auto id = data.first;
		auto value = data.second;

		c_setting* setting = nullptr;

		if ( !( setting = this->find_setting<c_setting>( id ) ) ) continue;

		setting->load_value( value );

	}

}

void settings::c_boolean::load_value( std::string line ) {

	if ( line == "true" || line == "1" || line == "enabled" ) this->value = true;
	else this->value = false;

}

std::string settings::c_boolean::export_value( void ) {

	if ( this->value ) return "true";
	
	return "false";

}

settings::c_float::c_float( std::string id, float value ) : c_setting(id) {

	this->value = value;

}

void settings::c_float::load_value( std::string string ) {

	bool is_negative = string[ 0 ] == '-';

	if ( is_negative ) string = string.substr( 1, string.size( ) );


	auto get_numbers = [ string ]( void ) {

		auto decimal = string.find_first_of( "." );

		if ( decimal == std::string::npos ) decimal = string.size( );

		return std::pair<std::string, std::string>{string.substr( 0, decimal - 1 ), string.substr( decimal + 1, string.size( ) )};

	};

	float result = 0.f;

	auto numbers = get_numbers( );

	for ( float i = 0; i < numbers.first.size( ); i++ ) {

		auto number = static_cast< float >( numbers.first[ i ] - '0' );

		if ( number > 9 || number < 0 ) continue;

		result += ( number * std::powf( 10.f, ( numbers.first.size( ) - i ) ) );

	}

	for ( float i = 0; i < numbers.second.size( ); i++ ) {

		auto number = static_cast< float >( numbers.second[ i ] - '0' );

		if ( number > 9 || number < 0 ) continue;

		result += ( number * std::powf( 10.f, -( i + 1 ) ) );

	}

	if ( is_negative ) result = -result;

	this->value = result;
}

std::string settings::c_float::export_value( void ) {

	return std::to_string( this->value );

}

settings::c_string::c_string( std::string id, std::string value ) : c_setting(id) {

	this->value = value;

}

std::string settings::c_string::export_value( void ) {

	return this->value;

}

void settings::c_string::load_value( std::string value ) {

	this->value = value;

}

settings::c_int::c_int( std::string id, int value ) : c_setting( id ) {

	this->value = value;

}

void settings::c_int::load_value( std::string string ) {

	bool is_negative = string[ 0 ] == '-';

	if ( is_negative ) string = string.substr( 1, string.size( ) );

	int result = 0;

	for ( int i = 0; i < string.size( ); i++ ) {

		auto number = static_cast< int >( string[ i ] - '0' );

		if ( number < 0 || number > 9 ) continue;

		result += ( number * static_cast< int >( std::powf( 10.f, static_cast< float >( string.size( ) - 1 - i ) ) ) );

	}

	if ( is_negative ) result = -result;

	this->value = result;

}

std::string settings::c_int::export_value( void ) {

	return std::to_string( this->value );

}