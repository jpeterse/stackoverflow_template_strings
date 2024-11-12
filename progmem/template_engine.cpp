#include "template_engine.h"

namespace template_engine {

  char sep_char;


  void begin() {
    sep_char = '@';
  }


  void setFieldSeperator( const char c ) {
    sep_char = c;
  }


  bool process_line( char* line, G_MATCH_T &match ) {
    bool _retVal = false;
    MatchState ms;

    if ( hasTemplate( ms, line) == REGEXP_MATCHED ) {
      // we found a template in the line 
      match.match_start = ms.MatchStart;
      match.match_length = ms.MatchLength;
      match.match_end = ms.MatchStart + ms.MatchLength;
      // so next we need to see if the template is just a variable, or if it contains any attributes
      parceTemplateEntry( ms, match );
      _retVal = true;
    }

    return _retVal;
  }


  // process searchString and return a MatchState object
  char hasTemplate( MatchState &ms, char* searchStr ) {
    char regExStr[ 18 ]; // will be set to "%@%@([^%@]+)%@%@" if default sep_char is used
    char _ret = REGEXP_NOMATCH;
    char _buf[ 0xff ];

    sprintf( regExStr, "%c%c([^%c]+)%c%c", sep_char, sep_char, sep_char, sep_char, sep_char );
      
    ms.Target( searchStr );
    return ms.Match( regExStr, 0 );
  }


  void parceTemplateEntry( MatchState &ms, G_MATCH_T &match ) {
    const uint8_t regExCount = 2;
    const char regExStr[ regExCount ][ 27 ] = { "(.*)::(.*)(::)(.*)", "(.*)::(.*)(==)(.*)" };
    char _buf[ 4 ][ match.match_length + 1 ];
    char _newTarget[ match.match_length + 1 ];
    MatchState ms2;

    ms.GetCapture( _newTarget, 0 );    // _buf[0] now contains the template string, e.g. @@value::attribute::attr_value@@

    ms2.Target( _newTarget );
    _buf[ 0 ][ 0 ] = '\0';  // clear
    char _ret = REGEXP_NOMATCH;
    uint8_t i = 0;
    while ( _ret == REGEXP_NOMATCH && i < regExCount )
      _ret = ms2.Match( regExStr[ i++ ] );

    if ( _ret == REGEXP_MATCHED ) {
      match.hasAttribute = ( i == 1 );
      match.hasCondition = ( i == 2 );

      ms2.GetCapture( _buf[ 0 ], 0 );
      ms2.GetCapture( _buf[ 1 ], 1 );
      match.variable_name.substr_start = match.match_start + 2;
      match.variable_name.substr_length = strlen( _buf[ 1 ] );
      match.variable_name.substr_end = match.variable_name.substr_start + match.variable_name.substr_length;
      ms2.GetCapture( _buf[ 2 ], 2 );
      match.attribute_name.substr_start = match.variable_name.substr_end + 2;
      match.attribute_name.substr_length = strlen( _buf[ 2 ] );
      match.attribute_name.substr_end = match.attribute_name.substr_start + match.attribute_name.substr_length;
      ms2.GetCapture( _buf[ 3 ], 3 );
      match.attribute_value.substr_start = match.attribute_name.substr_end + 2;
      match.attribute_value.substr_length = strlen( _buf[ 3 ] );
      match.attribute_value.substr_end = match.attribute_value.substr_start + match.attribute_value.substr_length;

    } else {
      match.hasAttribute = false;
      match.hasCondition = false;
      match.variable_name.substr_start = match.match_start + 2;
      match.variable_name.substr_length = match.match_length - 4;
      match.variable_name.substr_end = match.variable_name.substr_start + match.variable_name.substr_length;
      match.attribute_name.substr_start = 0;
      match.attribute_name.substr_length = 0;
      match.attribute_name.substr_end = 0;
      match.attribute_value.substr_start = 0;
      match.attribute_value.substr_length = 0;
      match.attribute_value.substr_end = 0;
    }
  }

} // namespace