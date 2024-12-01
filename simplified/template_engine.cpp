#include "template_engine.h"


namespace TMPL_ENGINE {

  /* TMPL_ENGINE is used to scan a haystack for one of the following patterns
        @@var@@
        @@var::attr::opt@@
        @@var::attr==val@@   // where "==" can be any comparison operator
    if a pattern is found, pointers to the location of each segment within the pattern is then recorded
    TMPL_ENGINE also contain a utility function, for replacing the pattern, with the content of a provided char array
  */

  static const uint8_t m_tmpl_parts_count = 4;
  enum tmpl_parts_e { m_tmpl_e, m_var_e, m_attr_e, m_opt_val_e };

  const char m_operator_chars[ 7 ] = "!=<>&|";

  struct tmpl_part_t {
    size_t start;
    size_t length;
    size_t end;
  };

  tmpl_part_t m_tmpl_parts[ m_tmpl_parts_count ];

  char m_tmpl_delim[ 3 ];
  char m_attr_delim[ 3 ];
  char m_operator[ 3 ];
  bool m_hasAttr = false;
  bool m_hasVal = false;


  /* initializer for TMPL_ENGINE. Sets default values for the needles to search for in haystack
  */
  void begin() {
    set_tmpl_delim_chr( '@' );
    set_attr_delim_chr( ':' );
    reset();
  }


  /* function that resets all pointer values to 0
  */
  void reset() {
    for ( uint8_t i = 0; i < m_tmpl_parts_count; i++ ) {
      m_tmpl_parts[ i ].start = 0;
      m_tmpl_parts[ i ].length = 0;
      m_tmpl_parts[ i ].end = 0;
    }
    m_operator[ 0 ] = '\0';
    m_hasAttr = false;
  }


  /* function the print content of m_tmpl_parts to serial
  */
  void debug_print() {
    Serial.printf( "%s\tStart: %zu\tLength: %zu\tEnd: %zu\n", "Template Pattern", m_tmpl_parts[ 0 ].start, m_tmpl_parts[ 0 ].length, m_tmpl_parts[ 0 ].end );
    Serial.printf( "%s\tStart: %zu\tLength: %zu\tEnd: %zu\n", "Variable        ", m_tmpl_parts[ 1 ].start, m_tmpl_parts[ 1 ].length, m_tmpl_parts[ 1 ].end );
    if ( m_hasAttr ) {
      Serial.printf( "%s\tStart: %zu\tLength: %zu\tEnd: %zu\n", "Attribute       ", m_tmpl_parts[ 2 ].start, m_tmpl_parts[ 2 ].length, m_tmpl_parts[ 2 ].end );
      Serial.printf( "%s\tStart: %zu\tLength: %zu\tEnd: %zu\n", "Option or Value ", m_tmpl_parts[ 3 ].start, m_tmpl_parts[ 3 ].length, m_tmpl_parts[ 3 ].end );
    }
  }


  /* function to set custom needle value, that defines the start and end of the pattern
    Note: the character are expected to appear in pairs, i.e. two successive characters at both
    the start and the end of the pattern.
    @param char, the character that defines the start or end of the pattern.
  */
  void set_tmpl_delim_chr( char c ) {
    sprintf( m_tmpl_delim, "%c%c", c, c );
  }


  /* function to set the needles value, that defines the seperator between the segment with pattern
    NOTE: the needle value is expected to appear in pairs, i.e. two successive delimeter characters are required
    @param char, the character that defines the end of one segment, and the start of the next segment
  */
  void set_attr_delim_chr( char c ) {
    sprintf( m_attr_delim, "%c%c", c, c );
  }


  /* function that searches haystack for the presense of a pattern
    @param haystack, character array for search
    @return boolean, true if pattern was found in haystack
  */
  bool hasTemplate( char* haystack ) {
    bool _retVal = false;
    size_t _start = 0;
    size_t _end = 0;

    if ( find_key( haystack, m_tmpl_delim, _start ) )                   // look for start of template string
      if ( find_key( &haystack[ _start + 2 ], m_tmpl_delim, _end ) ) {  // look for matching end of template string
        m_tmpl_parts[ m_tmpl_e ].start = _start;                        // start and end position are absolute string positions
        m_tmpl_parts[ m_var_e ].start = _start + 2;
        m_tmpl_parts[ m_var_e ].end = _start + 2 + _end;
        m_tmpl_parts[ m_var_e ].length = _end;
        m_tmpl_parts[ m_tmpl_e ].end = _start + 2 + _end + 2;
        m_tmpl_parts[ m_tmpl_e ].length = 2 + _end + 2;
        _retVal = true;
    }

    return _retVal;
  }


  /* function that scans pattern and identifies, if pattern is a simple string value, or if pattern contain segments.
    If segments are found, the start, end and length of each segment is then recorded.
    @param haystack, the char array to search
    @return boolean, true if pattern contain segments.
  */
  bool hasAttribute( char* haystack ) {
    size_t _first_delim, _second_delim;

    m_hasAttr = false;
    m_hasVal = false;
    _first_delim = 0;
    _second_delim = 0;

    // a valid template with options or attributes, must have two sets of separators
    // either two segment separators, or a segment separater followed by an operand separator

    // look for the first separator
    m_hasAttr = find_key( haystack + m_tmpl_parts[ m_var_e ].start, m_attr_delim, _first_delim );
    if ( m_hasAttr == false || _first_delim >= m_tmpl_parts[ m_tmpl_e ].length - 4 ) {
      m_hasAttr = false;
      return false;
    }

    // then look for the second separator
    m_hasAttr = find_key( haystack + m_tmpl_parts[ m_var_e ].start + _first_delim + 2, m_attr_delim, _second_delim );
    if ( !m_hasAttr || _second_delim >= m_tmpl_parts[ m_tmpl_e ].length - 4 ) {
      // second delimeter is not an option delimeter, check for comparison operators
      m_hasVal = find_operand( haystack + m_tmpl_parts[ m_var_e ].start + _first_delim + 2, _second_delim );
      if ( m_hasVal == false )
        return false;
    }

    // found separators
    m_hasAttr = true;

    // translate string positions to absolute positions
    _second_delim += m_tmpl_parts[ m_var_e ].start + _first_delim + 2;
    _first_delim += m_tmpl_parts[ m_var_e ].start;

    // record the start and end locations of each segment within pattern
    m_tmpl_parts[ m_var_e ].end = _first_delim;
    m_tmpl_parts[ m_var_e ].length = m_tmpl_parts[ m_var_e ].end - m_tmpl_parts[ m_var_e ].start;
    m_tmpl_parts[ m_attr_e ].start = _first_delim + 2;
    m_tmpl_parts[ m_attr_e ].end = _second_delim;
    m_tmpl_parts[ m_attr_e ].length = _second_delim - ( _first_delim + 2 );
    m_tmpl_parts[ m_opt_val_e ].start = _second_delim + 2;
    m_tmpl_parts[ m_opt_val_e ].end = m_tmpl_parts[ m_tmpl_e ].end - 2;
    m_tmpl_parts[ m_opt_val_e ].length = m_tmpl_parts[ m_opt_val_e ].end - m_tmpl_parts[ m_opt_val_e ].start;

    return true;
  }


  /* function that looks for the location of the beginning of a pattern entry.
    A pattern entry is define by the presence of two successive characters, defined by delim (default '@' )
    @param haystack, char array to search
    @param needle, char array defining the string value to search for
    @param pos, location of the first instance of needle within haystack
    #return boolean, true if needle was found in haystack, false needle is not in haystack
  */
  bool find_key( char* haystack, char* needle, size_t &pos ) {
    const char* _needle;

    _needle = strstr( haystack, needle );
    if ( _needle == nullptr ) {
      pos = 0;
      return false;
    }

    pos = _needle - haystack;
    return true;
  }


  /* function to locate an operand between a attribute and a value
    @param haystack, char array to search through
    @param pos, will be set to location of start of operand if found
    @return boolean, true if operand is found, false otherwise
  */
  bool find_operand( char* haystack, size_t &pos ) {
    char * _needle;

    _needle = strpbrk( haystack, m_operator_chars );
    if ( !_needle )
      return false;

    pos = _needle - haystack;
    return true;
  }


  /* function replaces content in haystack between needle_start and needle_end, with content provided in new_val.
    @param haystack, pointer to a char array containing the char array of the string to update. Haystack will be updated in place.
    @param new_val, pointer to a char array containing the value to insert into haystack
    @param needle_start, start location (counted from beginning of haystack) of the content to be replaced
    @param needle_end, end location (counted from beginning of haystack) of the content to be replaced
    @param haystack size, the total size of the allocated memory for haystack
    @return boolean true or false, if new_val was inserted into haystack or not.
  */
  bool replace_str( char* haystack, const char* new_val, uint16_t needle_start, uint16_t needle_end, uint16_t haystack_size ) {
    size_t haystack_length = strlen( haystack );  // current lenght of content within haystack
    size_t _new_val_length = strlen( new_val );

    if ( !checkBounds( haystack_size, haystack_length, (needle_end - needle_start ), _new_val_length ) )
      return false;

    if ( ( needle_end - needle_start ) != _new_val_length ) {
      memmove( (void*)(haystack + needle_start + _new_val_length), (void*)(haystack + needle_end), haystack_length - needle_end + 1);
    }

    memcpy( (void*)(haystack + needle_start), (void*)new_val, _new_val_length );
    return true;
  }


  /* function that checks if replacing a needle in haystack with new_val would write beyond the allocated memory of haystack.
    @param haystack_size, total allocated memory size of the haystack.
    @param haystack_length, length of the current content in haystack
    @param needle_length, length of the content to be replaced
    @param new_val_length, length of new content to be inserted
    @return boolean true or false, if new_val will fit within allocated memory size of haystack.
  */
  bool checkBounds( size_t haystack_size, size_t haystack_length, size_t needle_length, size_t new_val_length ) {
    if ( new_val_length <= needle_length )
      return true;
    if ( ( haystack_size - ( haystack_length + 1 ) ) > ( new_val_length - needle_length ) ) // this could be ">=", but playing it safe
      return true;
    Serial.println( "checkBounds returns false. New value would make haystack's content extend beyond haystack's allocated size" );
    return false;
  }


  /* function to get absolute string prosition of start of template
    @return absolute start of template string, counted as number of characters from beginning of line
  */
  size_t getMatchStart() { return m_tmpl_parts[ m_tmpl_e ].start; }
  /* function to get absolute string prosition of end of template
    @return absolute end of template string, counted as number of characters from beginning of line
  */
  size_t getMatchEnd() { return m_tmpl_parts[ m_tmpl_e ].end; }
  /* function to get length of template
    @return the length of the template string
  */
  size_t getMatchLength() { return m_tmpl_parts[ m_tmpl_e ].length; }

  /* function to get absolute string prosition of start of variable
    @return absolute start of variable string, counted as number of characters from beginning of line
  */
  size_t getVariableStart() { return m_tmpl_parts[ m_var_e ].start; }
  /* function to get absolute string prosition of end of variable
    @return absolute end of variable string, counted as number of characters from beginning of line
  */
  size_t getVariableEnd() { return m_tmpl_parts[ m_var_e ].end; }
  /* function to get length of variable
    @return the length of the variable string
  */
  size_t getVariableLength() { return m_tmpl_parts[ m_var_e ].length; }

  /* function to get absolute string prosition of start of attribute
    @return absolute start of attribute string, counted as number of characters from beginning of line
  */
  size_t getAttributeStart() { return m_tmpl_parts[ m_attr_e ].start; }
  /* function to get absolute string prosition of end of attribute
    @return absolute end of attribute string, counted as number of characters from beginning of line
  */
  size_t getAttributeEnd() { return m_tmpl_parts[ m_attr_e ].end; }
  /* function to get length of attribute
    @return the length of the attribute string
  */
  size_t getAttributeLength() { return m_tmpl_parts[ m_attr_e ].length; }

  /* function to get absolute string prosition of start of option
    @return absolute start of option string, counted as number of characters from beginning of line
  */
  size_t getOptionStart() { return m_tmpl_parts[ m_opt_val_e ].start; }
  /* function to get absolute string prosition of end of option
    @return absolute end of option string, counted as number of characters from beginning of line
  */
  size_t getOptionEnd() { return m_tmpl_parts[ m_opt_val_e ].end; }
  /* function to get length of option
    @return the length of the option string
  */
  size_t getOptionLength() { return m_tmpl_parts[ m_opt_val_e ].length; }

  /* function to get absolute string prosition of start of value
    @return absolute start of value string, counted as number of characters from beginning of line
  */
  size_t getValueStart() { return m_tmpl_parts[ m_opt_val_e ].start; }
  /* function to get absolute string prosition of end of value
    @return absolute end of value string, counted as number of characters from beginning of line
  */
  size_t getValueEnd() { return m_tmpl_parts[ m_opt_val_e ].end; }
  /* function to get length of value
    @return the length of the value string
  */
  size_t getValueLength() { return m_tmpl_parts[ m_opt_val_e ].length; }
}
