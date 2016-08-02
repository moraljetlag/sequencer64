#ifndef SEQ64_MIDI_CONTAINER_HPP
#define SEQ64_MIDI_CONTAINER_HPP

/*
 *  This file is part of seq24/sequencer64.
 *
 *  seq24 is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  seq24 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with seq24; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * \file          midi_container.hpp
 *
 *  This module declares the abstract base class for the management of some
 *  MIDI events, using the sequence class.
 *
 * \library       sequencer64 application
 * \author        Seq24 team; modifications by Chris Ahlstrom
 * \date          2015-10-10
 * \updates       2016-08-02
 * \license       GNU GPLv2 or above
 *
 */

#include <cstddef>                      /* std::size_t          */
#include <string>                       /* std::string          */

#include "app_limits.h"                 /* SEQ64_NULL_SEQUENCE  */
#include "midibyte.hpp"                 /* seq64::midibyte      */

/**
 *  This macro is used for detecting SeqSpec data that Sequencer64 does not
 *  handle.  If this word is found, then we simply extract the expected number
 *  of characters specified by that construct, and skip them when parsing a
 *  MIDI file.
 */

#define SEQ64_PROPTAG_HIGHWORD          0x24240000

/**
 *  An easier, shorter test for the SEQ64_PROPTAG_HIGHWORD part of a long
 *  value, that clients can use.
 */

#define SEQ64_IS_PROPTAG(p) \
    (((p) & SEQ64_PROPTAG_HIGHWORD) == SEQ64_PROPTAG_HIGHWORD)

/**
 *  The maximum sequence number, in macro form.  This value indicates that no
 *  background sequence value has been assigned yet.  See the value
 *  seqedit::m_initial_sequence, which was originally set to -1 directly.
 *  However, we have issues saving a negative number in MIDI, so we will use
 *  the "proprietary" track's bogus sequence number, which double the 1024
 *  sequences we can support.  Values between 0 (inclusive) and
 *  SEQ64_SEQUENCE_LIMIT (exclusive) are valid.  But SEQ64_SEQUENCE_LIMIT is a
 *  <i> legal</i> value, used only for disabling the selection of a background
 *  sequence.
 */

#define SEQ64_SEQUENCE_LIMIT            0x0800          /* 2048 */

/**
 *  A convenient macro function to test against SEQ64_SEQUENCE_LIMIT.
 *  This macro allows SEQ64_SEQUENCE_LIMIT as a legal value to use.
 */

#define SEQ64_IS_LEGAL_SEQUENCE(s)      ((s) >= 0 && (s) <= SEQ64_SEQUENCE_LIMIT)

/**
 *  A convenient macro function to test against SEQ64_SEQUENCE_LIMIT.
 *  This macro does not allow SEQ64_SEQUENCE_LIMIT as a valid value to use.
 */

#define SEQ64_IS_VALID_SEQUENCE(s)      ((s) >= 0 && (s) < SEQ64_SEQUENCE_LIMIT)

/**
 *  A convenient macro function to test against SEQ64_SEQUENCE_LIMIT.
 *  This macro does not allow SEQ64_SEQUENCE_LIMIT as a valid value to use.
 */

#define SEQ64_IS_DISABLED_SEQUENCE(s)   ((s) == SEQ64_SEQUENCE_LIMIT)

/**
 *  Provides a way to detect that a background sequence number that has not
 *  yet been assigned.
 */

#define SEQ64_IS_NULL_SEQUENCE(s)       ((s) == SEQ64_NULL_SEQUENCE)

namespace seq64
{
    class event;
    class sequence;
    class trigger;

/**
 *  Provides tags used by the midifile class to control the reading and
 *  writing of the extra "proprietary" information stored in a Seq24 MIDI
 *  file.  Some of the information is stored with each track (and in the
 *  midi_container-derived classes), and some is stored in the proprietary
 *  header.
 *
 *  Track (sequencer-specific) data:
 *
\verbatim
            c_midibus
            c_midich
            c_timesig
            c_triggers (deprecated)
            c_triggers_new
            c_musickey (can be in footer, as well)
            c_musicscale (ditto)
            c_backsequence (ditto)
            c_transpose
\endverbatim
 *
 * Footer ("proprietary") data:
 *
\verbatim
            c_midictrl
            c_midiclocks
            c_notes
            c_bpmtag (beats per minute)
            c_mutegroups
\endverbatim
 *
 *  Also see the PDF file in the following project for more information about
 *  the "proprietary" data:
 *
 *      https://github.com/ahlstromcj/sequencer64-doc.git
 *
 *  Note that the track data is read from the MIDI file, but not written
 *  directly to the MIDI file.  Instead, it is stored in the MIDI container as
 *  sequences are edited to used these "sequencer-specific" features.
 *  Also note that c_triggers has been replaced by c_triggers_new as the code
 *  that marks the triggers stored with a sequence.
 *
 *  As an extension, we can also grab the key, scale, and background sequence
 *  value selected in a sequence and write these values as track data, where
 *  they can be read in and applied to a specific sequence, when the seqedit
 *  object is created.  These values would not be stored in the legacy format.
 *
 *  Something like this could be done in the "user" configuration file, but
 *  then the key and scale would apply to all songs.  We don't want that.
 *
 *  We could also add snap and note-length to the per-song defaults, but
 *  the "user" configuration file seems like a better place to store these
 *  preferences.
 */

const midilong c_midibus =      0x24240001; /**< Track buss number.         */
const midilong c_midich =       0x24240002; /**< Track channel number.      */
const midilong c_midiclocks =   0x24240003; /**< Track clocking.            */
const midilong c_triggers =     0x24240004; /**< See c_triggers_new.        */
const midilong c_notes =        0x24240005; /**< Song data.                 */
const midilong c_timesig =      0x24240006; /**< Track time signature.      */
const midilong c_bpmtag =       0x24240007; /**< Song beats/minute.         */
const midilong c_triggers_new = 0x24240008; /**< Track trigger data.        */
const midilong c_mutegroups =   0x24240009; /**< Song mute group data.      */
const midilong c_midictrl =     0x24240010; /**< Song MIDI control.         */
const midilong c_musickey =     0x24240011; /**< The track's key.           */
const midilong c_musicscale =   0x24240012; /**< The track's scale.         */
const midilong c_backsequence = 0x24240013; /**< Track background sequence. */

#ifdef SEQ64_STAZED_TRANSPOSE
const midilong c_transpose =    0x24240014; /**< Track transpose value.     */
#endif

/**
 *    This class is the abstract base class for a container of MIDI track
 *    information.  It is the base class for midi_list and midi_vector.
 */

class midi_container
{
    friend class midifile;

private:

    /**
     *  Provide a hook into a sequence so that we can exchange data with a
     *  sequence object.
     */

    sequence & m_sequence;

    /**
     *  Provides the position in the container when making a series of get()
     *  calls on the container.
     */

    mutable unsigned int m_position_for_get;

public:

    midi_container (sequence & seq);

    /**
     *  A rote constructor needed for a base class.
     */

    virtual ~midi_container()
    {
        // empty body
    }

    void fill (int tracknumber);

    /**
     *  Returns the size of the container, in midibytes.  Must be overridden
     *  in the derived class, though not pure.
     */

    virtual std::size_t size () const
    {
        return 0;
    }

    /**
     *  Instead of checking for the size of the container when "emptying" it
     *  [see the midifile::write() function], use this function, which is
     *  overridden to match the type of container being used.
     */

    virtual bool done () const
    {
        return true;
    }

    /**
     *  Provides a way to add a MIDI byte into the container.  The original
     *  seq24 container used an std::list and a push_front operation.
     */

    virtual void put (midibyte b) = 0;

    /**
     *  Provide a way to get the next byte from the container.  It also
     *  increments m_position_for_get.
     */

    virtual midibyte get () = 0;

    /**
     *  Provides a way to clear the container.
     */

    virtual void clear () = 0;

protected:

    /**
     * \setter m_position_for_get
     *      Sets the position to 0 and then returns that value.
     */

    unsigned int position_reset () const
    {
        m_position_for_get = 0;
        return m_position_for_get;
    }

    /**
     * \getter m_position_for_get
     *      Returns the current position.
     */

    unsigned int position () const
    {
        return m_position_for_get;
    }

    /**
     * \getter m_position_for_get
     *      Increments the current position.
     */

    void position_increment () const
    {
        ++m_position_for_get;
    }

private:

    void add_variable (midipulse v);
    void add_long (midipulse x);
    void add_short (midishort x);
    void add_event (const event & e, midipulse deltatime);
    void fill_seq_number (int seq);
    void fill_seq_name (const std::string & name);
    void fill_meta_track_end (midipulse deltatime);
    void fill_proprietary ();
    void fill_time_sig_and_tempo ();
    midipulse song_fill_seq_event (const trigger & trig, midipulse prev_timestamp);
    void song_fill_seq_trigger
    (
        const trigger & trig, midipulse len, midipulse prev_timestamp
    );

};

}           // namespace seq64

#endif      // SEQ64_MIDI_CONTAINER_HPP

/*
 * midi_container.hpp
 *
 * vim: sw=4 ts=4 wm=4 et ft=cpp
 */

