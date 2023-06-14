<?php

/*
* Author: Tomáš Tlapák
* login: xltapa00
* Year: 2020
*/

$loc = 0; //rows with instruction
$comments = 0; //rows with comment
$labels = array(); //count of defined unique labels
$jumps = 0; // count of jumps (call, return...)
$output_stats = array(); //array of arguments values, serves for extension STATP
$path = NULL; // path to file. that stores values of STATP extension

/*
* Parsing aruguments with getopt function
*/
function arguments($argc, $argv)
{   
    global $output_stats, $path;

    if ($argc == 1) return;
    $shortopts  = "";
    $longopts  = array(
        "stats:",          // Required value
        "help",            // No value
        "loc",             // No value
        "comments",        // No value
        "labels",          // No value
        "jumps",           // No value
    );

    $options = getopt($shortopts, $longopts);

    
    for ($i = 1; $i <= $argc-1; $i++)
    {
      if (preg_match("/^(--help|--loc|--comments|--jumps|--labels|--stats=[\w|\W]+)$/u", $argv[$i], $matches) == 0)
      {
        fwrite (STDERR, "Bad argument given. Use argument [--help] for help message.\n");
        exit(10);
      }
    }

    $help = false; 
    $stats = false;
    foreach (array_keys($options) as $value) 
    {
      if (is_array($options[$value])) 
      { 
        $tmp = count($options[$value]);
        for ($i=0; $i < $tmp ; $i++) 
        {
          array_push($output_stats, $value); 
        }
      }
      else 
      {
        switch ($value) 
        {
            case 'help':
                $help = true;
                break;
            case 'stats':
                $stats = true;
                $path = $options['stats'];   
                break;
            case 'loc':
            case 'comments':
            case 'labels':
            case 'jumps':
                array_push($output_stats, $value);
                break;
        }
      }
    }
    if ($help == true && $argc == 2) 
    {
      fwrite (STDOUT, "Skript načte ze standardního vstupu zdrojový kód v IPPcode19, 
      zkontroluje lexikální a syntaktickou správnost kódu a vypíše na standardní
      výstup XML reprezentaci programu dle specifikace v sekci 3.1.\n
      Skript pracuje s argmenty:
      --help
      --stats=<cesta k souboru>
      --loc -zaznamená počet řádků obsahujicí instrukci
      --comments -zaznamená počer řádků obsahující komentář
      --labels -zaznamená počet definovaných unikátních návěští
      --jumps -zaznamená počet přimých nebo nepřímých skoků\n
      Argument --help nejde kombinovat s ostatnimí argumenty!
      Při použití některého z argumentů --labels | --loc | --comments | --jumps je nutné
      použít i argument --stats!\n");
        exit(0);
    }

    if ($help == true && $argc > 2)
    {
        fwrite (STDERR, "It's not allowed to combine --help with others arguments.\n");
        exit(10);
    }

    if ($stats == false && $argc > 2)
    {
        fwrite (STDERR, "Argument --stats is not used, can't save statistics to file.\n");
        exit(10);
    }
}

/*
* Function for extension STATP. Writes data to file specified by variable $path
*/
function output()
{
  global $path, $output_stats, $loc, $comments, $labels, $jumps;
  $output = NULL;
  foreach ($output_stats as &$value)  
  {
    if ($value == "loc")
    {
      $output .= "$loc\n";        
    }
    elseif($value == "comments")
    {
      $output .= "$comments\n";    
    }
    elseif($value == "labels")
    { 
      $labels = sizeof(array_unique($labels));
      $output .= "$labels\n"; 
    }
    else
    {
      $output .= "$jumps\n";
    }
  }
  #path exists
  if (realpath($path))
  { 
    if ($file = @fopen($path, "w"))
    {
      fwrite($file, $output);
      fclose($file); 
    }
    else
    {
      $path = realpath($path);
      $base = basename($path);
      fwrite (STDERR, "Couldn't open $path. $base is not a file or permission denied.\n");
      exit(12);
    }
  }
  #doesnt exist
  else
  { 
    if (realpath(dirname($path)))
    {
      if (@touch($path))
      {
        $file = fopen($path, "w");
        fwrite($file, $output);
        fclose($file);
      }
      else
      {
        fwrite (STDERR,"Couldn't create file.\n");
        exit(12);
      }
    }
    #trying create folder with potencial subfolders and file
    elseif (@mkdir(dirname($path), 0777, true))
    {
      if (@touch($path))
      {
        $file = fopen($path, "w");
        fwrite($file, $output);
        fclose($file);
      }
      else
      {
        fwrite (STDERR,"Couldn't create file.\n");
        exit(12);
      }
    }
    else
    {
      fwrite (STDERR,"Couldn't create file.\n");
      exit(12);
    }
  }
}

/*
* Checks valid header format for IPPCode20 language
*/
function head()
{
    global $comments;
    while ($line = fgets(STDIN))
    {
      #emtpy line
      if (preg_match("/^(?:\ |\t|\n)*$/iu", $line))
      {
          continue;
      }
      #comment
      if (preg_match("/^(?:\ |\t)*#+[^\n]*$/iu", $line))
      {
          $comments++;
          continue;
      }
      #valid header
      if (preg_match("/^(?:\ |\t)*(\.ippcode20)(?:\ |\t)*$/iu", $line))
      {
          return;
      }
      #valid header with comment
      elseif (preg_match("/^(?:\ |\t)*(\.ippcode20)(?:\ |\t)*#+[^\n]*$/iu", $line)) 
      {
        $comments++;
        return;
      }
      else
      {
          fwrite (STDERR,"Missing or bad header.\n");
          exit(21);
      }
    }
    fwrite (STDERR,"Missing or bad header.\n");
    exit(21);
}

/*
* Function, that generate XML, uses XMLWriter
 \param $xw - XMLWriter memory
 \param order - order of instructions in IPPCode20
 \param matches - matches from preg_match from function parse()
*/
function XML_generator($xw, $order, $matches)
{
    xmlwriter_start_element($xw, 'instruction');
    xmlwriter_write_attribute($xw, 'order', "$order");
    xmlwriter_write_attribute($xw, 'opcode', strtoupper($matches[1]));
    xmlwriter_end_attribute($xw);
    
    switch(strtoupper($matches[1]))
    {           
        #⟨var⟩
        case "DEFVAR": 
        case "POPS":
          xmlwriter_start_element($xw, 'arg1');
          xmlwriter_write_attribute($xw, 'type', 'var');
          xmlwriter_text($xw, "$matches[2]");
          xmlwriter_end_element($xw);  
        break;

        #⟨label⟩
        case "CALL":
        case "LABEL":
        case "JUMP":
          xmlwriter_start_element($xw, 'arg1');
          xmlwriter_write_attribute($xw, 'type', 'label');
          xmlwriter_text($xw, "$matches[2]");
          xmlwriter_end_element($xw);
        break;

        #⟨var⟩ ⟨symb⟩
        case "TYPE":
        case "NOT": 
        case "INT2CHAR":
        case "MOVE":
        case "STRLEN";
          xmlwriter_start_element($xw, 'arg1');
          xmlwriter_write_attribute($xw, 'type', 'var');
          xmlwriter_text($xw, "$matches[2]");
          xmlwriter_end_element($xw);
          xmlwriter_start_element($xw, 'arg2');
          $symb = explode('@',$matches[3], 2);
          if($symb[0] == "GF" || $symb[0] == "LF" || $symb[0] == "TF")
          {
            $symb[0] = "var";
            xmlwriter_write_attribute($xw, 'type', $symb[0]);
            xmlwriter_text($xw, $matches[3]);
          }
          else 
          {
          xmlwriter_write_attribute($xw, 'type', $symb[0]);
          xmlwriter_text($xw, "$symb[1]");
          }
          xmlwriter_end_element($xw);

        break;

        #⟨symb⟩
        case "WRITE": 
        case "PUSHS": 
        case "EXIT": 
        case "DPRINT":
          xmlwriter_start_element($xw, 'arg1');
          $symb = explode('@',$matches[2], 2);
          if($symb[0] == "GF" || $symb[0] == "LF" || $symb[0] == "TF")
          {
            $symb[0] = "var";
            xmlwriter_write_attribute($xw, 'type', $symb[0]);
            xmlwriter_text($xw, $matches[2]);
          }
          else 
          {
          xmlwriter_write_attribute($xw, 'type', $symb[0]);
          xmlwriter_text($xw, "$symb[1]");
          }
          xmlwriter_end_element($xw);
        break;

        #⟨var⟩ ⟨type⟩
        case "READ":
          xmlwriter_start_element($xw, 'arg1');
          xmlwriter_write_attribute($xw, 'type', 'var');
          xmlwriter_text($xw, "$matches[2]");
          xmlwriter_end_element($xw);
          xmlwriter_start_element($xw, 'arg2');
          xmlwriter_write_attribute($xw, 'type', 'type');
          xmlwriter_text($xw, "$matches[3]");
          xmlwriter_end_element($xw);
        break;
        
        #⟨var⟩ ⟨symb1⟩ ⟨symb2⟩
        case "AND":
        case "OR": 
        case "STRI2INT":
        case "CONCAT":  
        case "ADD": 
        case "SUB": 
        case "MUL": 
        case "IDIV": 
        case "LT":
        case "GT":
        case "EQ": 
        case "GETCHAR": 
        case "SETCHAR";
          xmlwriter_start_element($xw, 'arg1');
          xmlwriter_write_attribute($xw, 'type', 'var');
          xmlwriter_text($xw, "$matches[2]");
          xmlwriter_end_element($xw);
          xmlwriter_start_element($xw, 'arg2');
          $symb = explode('@',$matches[3], 2);
          if($symb[0] == "GF" || $symb[0] == "LF" || $symb[0] == "TF")
          {
            $symb[0] = "var";
            xmlwriter_write_attribute($xw, 'type', $symb[0]);
            xmlwriter_text($xw, $matches[3]);
          }
          else 
          {
          xmlwriter_write_attribute($xw, 'type', $symb[0]);
          xmlwriter_text($xw, "$symb[1]");
          }
          xmlwriter_end_element($xw);
          xmlwriter_start_element($xw, 'arg3');
          $symb = explode('@',$matches[4], 2);
          if($symb[0] == "GF" || $symb[0] == "LF" || $symb[0] == "TF")
          {
            $symb[0] = "var";
            xmlwriter_write_attribute($xw, 'type', $symb[0]);
            xmlwriter_text($xw, $matches[4]);
          }
          else 
          {
          xmlwriter_write_attribute($xw, 'type', $symb[0]);
          xmlwriter_text($xw, "$symb[1]");
          }
          xmlwriter_end_element($xw);  
        break;

        #⟨label⟩ ⟨symb1⟩ ⟨symb2⟩
        case "JUMPIFEQ": 
        case "JUMPIFNEQ":
          xmlwriter_start_element($xw, 'arg1');
          xmlwriter_write_attribute($xw, 'type', 'label');
          xmlwriter_text($xw, "$matches[2]");
          xmlwriter_end_element($xw);
          xmlwriter_start_element($xw, 'arg2');
          $symb = explode('@',$matches[3], 2);
          if($symb[0] == "GF" || $symb[0] == "LF" || $symb[0] == "TF")
          {
            $symb[0] = "var";
            xmlwriter_write_attribute($xw, 'type', $symb[0]);
            xmlwriter_text($xw, $matches[3]);
          }
          else 
          {
          xmlwriter_write_attribute($xw, 'type', $symb[0]);
          xmlwriter_text($xw, "$symb[1]");
          }
          xmlwriter_end_element($xw);
          xmlwriter_start_element($xw, 'arg3');
          $symb = explode('@',$matches[4], 2);
          if($symb[0] == "GF" || $symb[0] == "LF" || $symb[0] == "TF")
          {
            $symb[0] = "var";
            xmlwriter_write_attribute($xw, 'type', $symb[0]);
            xmlwriter_text($xw, $matches[4]);
          }
          else 
          {
          xmlwriter_write_attribute($xw, 'type', $symb[0]);
          xmlwriter_text($xw, "$symb[1]");
          }
          xmlwriter_end_element($xw);
        break;
        
        default:;
    }

    xmlwriter_end_element($xw);
}

/*
* Reads input from STDIN, checks valid syntax of IPPCode20
*/
function parse()
{   
    global $comments, $loc, $jumps, $labels, $path;
    
    $xw = xmlwriter_open_memory();
    xmlwriter_set_indent($xw, true);
    $res = xmlwriter_set_indent_string($xw, "    ");
    xmlwriter_start_document($xw, '1.0', 'UTF-8');
    xmlwriter_start_element($xw, 'program');
    xmlwriter_write_attribute($xw, 'language', 'IPPcode20');
  
    $order = 1;
    while ($line = fgets(STDIN))
    { 
        #split line by #
        $line =preg_split("/#/iu", $line);
        if((sizeof($line)) == 2) 
        {
            $comments++;
        }
        $first_word = strtok(strtoupper(rtrim($line[0],"\n"))," ");  
        $line = $line[0]; //array to string
        #line with no instrunction
        if ($first_word == "")
        {
            continue;
        }
        else
        {
          switch($first_word)
          {           
              case "CREATEFRAME":
              case "PUSHFRAME":
              case "POPFRAME":
              case "RETURN":
              case "BREAK":
                $loc++;
                if ($first_word == "RETURN")
                {
                    $jumps++;
                }
                if (preg_match("/^(?:\ |\t)*(CREATEFRAME|PUSHFRAME|POPFRAME|RETURN|BREAK)(?:\ |\t)*$/ui", $line, $matches))
                {
                    XML_generator($xw, $order, $matches);
                }
                else 
                {
                    fwrite(STDERR,"Lexical or syntax error\n");
                    exit(23);
                }
              break;

              #⟨var⟩
              case "DEFVAR": 
              case "POPS":
                $loc++;
                if (preg_match("/^(?:\ |\t)*(?i)(DEFVAR|POPS)(?-i)(?:\ |\t)+((?:GF|TF|LF)@(?:[\p{L}|\_|\*|\-|\$|\%|\!|\?|\&])(?:[\w|\_|\*|\-|\$|\%|\!|\?|\&])*)(?:\ |\t)*$/u", $line, $matches))
                {
                    XML_generator($xw, $order, $matches);
                }
                else 
                {
                    fwrite(STDERR,"Lexical or syntax error\n");
                    exit(23);
                }
              break; 

              #⟨label⟩
              case "CALL":
              case "LABEL":
              case "JUMP":
                $loc++;
                if ($first_word == "CALL" || $first_word == "JUMP")
                {
                  $jumps++;
                }
                if (preg_match("/^(?:\ |\t)*(?i)(CALL|LABEL|JUMP)(?-i)(?:\ |\t)+((?:[\p{L}|\_|\*|\-|\$|\%|\!|\?|\&])(?:[\w|\_|\*|\-|\$|\%|\!|\?|\&])*)(?:\ |\t)*$/u", $line, $matches))
                {
                    XML_generator($xw, $order, $matches);
                }
                else 
                {
                    fwrite(STDERR,"Lexical or syntax error\n");
                    exit(23);
                }
                if ($first_word == "LABEL")
                {
                  array_push($labels, $matches[2]);
                }
              break;

              #⟨var⟩ ⟨symb⟩
              case "TYPE":
              case "NOT": 
              case "INT2CHAR":
              case "MOVE":
              case "STRLEN":
                $loc++;
                if (preg_match("/^(?:\ |\t)*(?i)(TYPE|NOT|INT2CHAR|MOVE|STRLEN)(?-i)(?:\ |\t)+((?:GF|TF|LF)@(?:[\p{L}|\_|\*|\-|\$|\%|\!|\?|\&])(?:[\w|\_|\*|\-|\$|\%|\!|\?|\&])*)(?:\ |\t)+(((?:GF|TF|LF)@(?:[\p{L}|\_|\*|\-|\$|\%|\!|\?|\&])(?:[\w|\_|\*|\-|\$|\%|\!|\?|\&])*)|(?:bool@(?:true|false))|(?:nil@nil)|(?:int@(?:\+|-){0,1}\d+)|(?:string@(?:[^\x01-\x1F\#\s\\\\]|\\\\\d{3,})*))(?:\ |\t)*$/u", $line, $matches))
                {
                    XML_generator($xw, $order, $matches);
                }
                else 
                {
                    fwrite(STDERR,"Lexical or syntax error\n");
                    exit(23);
                }
              break;

              #⟨symb⟩
              case "WRITE": 
              case "PUSHS": 
              case "EXIT": 
              case "DPRINT":
                $loc++;
                if (preg_match("/^(?:\ |\t)*(?i)(WRITE|PUSHS|EXIT|DPRINT)(?-i)(?:\ |\t)+((?:(?:GF|TF|LF)@(?:[\p{L}|\_|\*|\-|\$|\%|\!|\?|\&])(?:[\w|\_|\*|\-|\$|\%|\!|\?|\&])*)|(?:bool@(?:true|false))|(?:nil@nil)|(?:int@(?:\+|-){0,1}\d+)|(?:string@(?:[^\x01-\x1F\#\s\\\\]|\\\\\d{3,})*))(?:\ |\t)*$/u", $line, $matches))
                {
                    XML_generator($xw, $order, $matches);
                }
                else 
                {
                    fwrite(STDERR,"Lexical or syntax error\n");
                    exit(23);
                }
              break;
             
              #⟨var⟩ ⟨type⟩
              case "READ":
                $loc++;
                if (preg_match("/^(?:\ |\t)*(?i)(READ)(?-i)(?:\ |\t)+((?:GF|TF|LF)@(?:[\p{L}|\_|\*|\-|\$|\%|\!|\?|\&])(?:[\w|\_|\*|\-|\$|\%|\!|\?|\&])*)(?:\ |\t)+(int|string|bool)(?:\ |\t)*$/u", $line, $matches))
                {
                    XML_generator($xw, $order, $matches);
                }
                else 
                {
                    fwrite(STDERR,"Lexical or syntax error\n");
                    exit(23);
                }
              break;
              
              #⟨var⟩ ⟨symb1⟩ ⟨symb2⟩
              case "AND":
              case "OR": 
              case "STRI2INT":
              case "CONCAT":  
              case "ADD": 
              case "SUB": 
              case "MUL": 
              case "IDIV": 
              case "LT":
              case "GT":
              case "EQ": 
              case "GETCHAR": 
              case "SETCHAR":
                $loc++;
                if (preg_match("/^(?:\ |\t)*(?i)(AND|OR|STRI2INT|CONCAT|ADD|SUB|MUL|IDIV|LT|GT|EQ|GETCHAR|SETCHAR)(?-i)(?:\ |\t)+((?:GF|TF|LF)@(?:[\p{L}|\_|\*|\-|\$|\%|\!|\?|\&])(?:[\w|\_|\*|\-|\$|\%|\!|\?|\&])*)(?:\ |\t)+((?:(?:GF|TF|LF)@(?:[\p{L}|\_|\*|\-|\$|\%|\!|\?|\&])(?:[\w|\_|\*|\-|\$|\%|\!|\?|\&])*)|(?:bool@(?:true|false))|(?:nil@nil)|(?:int@(?:\+|-){0,1}\d+)|(?:string@(?:[^\x01-\x1F\#\s\\\\]|\\\\\d{3,})*))(?:\ |\t)+((?:(?:GF|TF|LF)@(?:[\p{L}|\_|\*|\-|\$|\%|\!|\?|\&])(?:[\w|\_|\*|\-|\$|\%|\!|\?|\&])*)|(?:bool@(?:true|false))|(?:nil@nil)|(?:int@(?:\+|-){0,1}\d+)|(?:string@(?:[^\x01-\x1F\#\s\\\\]|\\\\\d{3,})*))(?:\ |\t)*$/u", $line, $matches))
                {
                    XML_generator($xw, $order, $matches);
                }
                else 
                {
                    fwrite(STDERR,"Lexical or syntax error\n");
                    exit(23);
                }
              break;
              
              #⟨label⟩ ⟨symb1⟩ ⟨symb2⟩
              case "JUMPIFEQ": 
              case "JUMPIFNEQ":
                $loc++;
                $jumps++;
                if (preg_match("/^(?:\ |\t)*(?i)(JUMPIFEQ|JUMPIFNEQ)(?-i)(?:\ |\t)+((?:[\p{L}|\_|\*|\-|\$|\%|\!|\?|\&])(?:[\w|\_|\*|\-|\$|\%|\!|\?|\&])*)(?:\ |\t)+((?:(?:GF|TF|LF)@(?:[\p{L}|\_|\*|\-|\$|\%|\!|\?|\&])(?:[\w|\_|\*|\-|\$|\%|\!|\?|\&])*)|(?:bool@(?:true|false))|(?:nil@nil)|(?:int@(?:\+|-){0,1}\d+)|(?:string@(?:[^\x01-\x1F\#\s\\\\]|\\\\\d{3,})*))(?:\ |\t)+((?:(?:GF|TF|LF)@(?:[\p{L}|\_|\*|\-|\$|\%|\!|\?|\&])(?:[\w|\_|\*|\-|\$|\%|\!|\?|\&])*)|(?:bool@(?:true|false))|(?:nil@nil)|(?:int@(?:\+|-){0,1}\d+)|(?:string@(?:[^\x01-\x1F\#\s\\\\]|\\\\\d{3,})*))(?:\ |\t)*$/u", $line, $matches))
                {
                    XML_generator($xw, $order, $matches);
                }
                else 
                {
                    fwrite(STDERR,"Lexical or syntax error\n");
                    exit(23);
                }
              break;

              default: 
              fwrite(STDERR, "Unknown or bad operation code.\n");
              exit(22);
          }
        $order++;
        }
    }
    xmlwriter_end_element($xw);
    xmlwriter_end_document($xw);
    if ($path != NULL)
    {
        output();
    }
    fwrite(STDOUT, str_replace("'", "&apos", xmlwriter_output_memory($xw)));
}

arguments($argc, $argv);
head();
parse();
?>