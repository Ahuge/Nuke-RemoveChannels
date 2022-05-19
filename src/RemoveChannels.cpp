// RemoveChannels.cpp
// Alex Hughes (2016)
// Version 1.01

/* 
    Remove channels from the input. 
    This is based off of the Remove.C example that The Foundry provided.

    It takes your regular expression and will test it against each channel
	in the in the current stream. It can be used to parse channels progromatically
	instead of having to explicitly set the Remove node's channels.
*/


#include "DDImage/NoIop.h"
#include "DDImage/Knobs.h" // Here we have our UI elements:
#include <regex>

using namespace DD::Image;

static const char* const CLASS = "RemoveChannels";
static const char* const HELP = ("Removes color channels from the image based "
										             "on the regular expression provided.\n\n"
										             "For a basic description of regular expressions, click this question mark.");

// -------------------- Header -------------------- 
class RemoveChannels : public NoIop
{
public:

  //! Default constructor.
  RemoveChannels (Node* node) : NoIop(node)
  {
    this->_message = "\\w+";
    this->operation = 1;
  }
  
  //! Virtual destructor.
  virtual ~RemoveChannels () {}
 
  //! This function does all the work.
  void _validate(bool) override;
  //! Defines the knobs for this node.
  virtual void knobs(Knob_Callback) override;
  
  //! Return the name of the class.
  const char* Class() const { return CLASS; }
  const char* node_help() const { return HELP; }
  static const Description description;

private: 
  //! Information private for the node.
  ChannelSet channels;
  std::regex rgx;
  const char* _message;
  int operation; // 0 = remove, 1 = keep
};

// -------------------- Implementation --------------------

static const char* const enums[] = {
  "remove", "keep", 0
};

void RemoveChannels::_validate(bool for_real)
{
  if (!this->_message) // Fast return if you don't have anything in there.
  {
	  set_out_channels(Mask_None); // Tell Nuke we didn't touch anything.
	  return;
  }

  this->copy_info(); // Get context of the channels that this node knows.

  ChannelMask inputChannels = this->input0().info().channels(); // Get all availible channels.
  try {
      
      if (knob("regular_expression")->get_text() != NULL) {
          this->rgx.assign(knob("regular_expression")->get_text());
      } else {
          this->rgx.assign("");
      }    

	  foreach(c, inputChannels)
	  {
         
		const std::string channelName = getName(c);  // String name of the channel so we can go begin and end on it.
		std::smatch match; // Our capture.

		if (this->operation) { // Keep matching channels

			// Regex matching.
			if (std::regex_search(channelName.begin(), channelName.end(), match, this->rgx))
			{
				this->info_.turn_on(c);   //? Tells that channel to turn on.
			}
			else
			{
				// Doesn't match
				this->info_.turn_off(c);   //? Tells that channel to turn off.
			}
		}
		else // Remove matching channels
		{
			// Regex matching.
			if (std::regex_search(channelName.begin(), channelName.end(), match, this->rgx))
			{
				this->info_.turn_off(c);   //? Tells that channel to turn off.
			}

			else
			{
				// Doesn't match
				this->info_.turn_on(c);   //? Tells that channel to turn on.
			}
		}
	  }
  }
  catch (std::regex_error& e) {
      // Syntax error in the regular expression
	  this->error("Syntax error in the regular expression.");
  }
  this->set_out_channels(channels); //? Tells nuke what we changed.
}

void RemoveChannels::knobs(Knob_Callback f)
{
    Enumeration_knob(f, &operation, enums, "operation", "Operation");
    Tooltip(f, "Remove:\tthe named channels are deleted\n\n"
               "Keep:\tall but the named channels are deleted");
    String_knob(f, &_message, "regular_expression", "Regular Expression");
    Tooltip(f, "This regular expression will be computed against each channel in the scene. "
               "This is channel's not layers, so you should be expecting rgba.red rgba.blue etc.\n\n"
			   "The only trick is that you have to double escape your backslashes because C++ "
			   "will eat them if there is only one slash.");
}

static Iop* build(Node* node)
{
    // Build the node.
    return new RemoveChannels(node);
}
const Iop::Description RemoveChannels::description(CLASS, "Channel/RemoveChannels", build);
